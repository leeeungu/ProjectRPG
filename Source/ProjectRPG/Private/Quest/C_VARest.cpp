#include "Quest/C_VARest.h"
#include "VaRestSubsystem.h"
#include "VaRestRequestJSON.h"
#include "VaRestTypes.h"
#include "VaRestJsonObject.h"
#include "VaRestJsonValue.h"

void UC_VARest::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	VaRestSubsystem = GEngine->GetEngineSubsystem<UVaRestSubsystem>();
	APIKey = TEXT("YOUR_API_KEY_HERE");
	
}

void UC_VARest::SendGPTRequest(FGPTRequest RequestData, TScriptInterface<IC_GPTInterface> Interface, UObject* Target)
{
	if (!Target)
		return;
	if (!VaRestSubsystem)
		VaRestSubsystem = GEngine->GetEngineSubsystem<UVaRestSubsystem>();
	if (!VaRestSubsystem)
		return;
	if (RequestData.Frompt.IsEmpty() || RequestData.Text.IsEmpty() || 
		RequestData.Text.Contains("Http", ESearchCase::IgnoreCase) || RequestData.Frompt.Contains("Http", ESearchCase::IgnoreCase) )
		//|| RequestData.Frompt.Contains("Http") >= 0 || RequestData.Frompt.Contains("http") >= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("GPT Request Invalid: %s"), *RequestData.Text);
		return;
	}
	if (APIKey == TEXT("YOUR_API_KEY_HERE"))
	{
		UE_LOG(LogTemp, Warning, TEXT("GPT API Key is Empty"));
		return;
	}
	UVaRestRequestJSON* Request = VaRestSubsystem->ConstructVaRestRequestExt(EVaRestRequestVerb::POST, EVaRestRequestContentType::json);
	{
		UVaRestJsonObject* RequestObject = VaRestSubsystem->ConstructVaRestJsonObject();
		{
			RequestObject->SetStringField(TEXT("model"), TEXT("gpt-4.1"));
		}
		{
			TArray< UVaRestJsonValue*> array{};
			UVaRestJsonObject* RequestValueObj = VaRestSubsystem->ConstructVaRestJsonObject();
			RequestValueObj->SetStringField(TEXT("role"), TEXT("developer"));
			RequestValueObj->SetStringField(TEXT("content"), RequestData.Frompt);
			array.Add(VaRestSubsystem->ConstructJsonValueObject(RequestValueObj));
			RequestValueObj = VaRestSubsystem->ConstructVaRestJsonObject();
			RequestValueObj->SetStringField(TEXT("role"), TEXT("user"));
			RequestValueObj->SetStringField(TEXT("content"), RequestData.Text);
			array.Add(VaRestSubsystem->ConstructJsonValueObject(RequestValueObj));
			RequestObject->SetArrayField(TEXT("input"), array);
		}

		Request->SetRequestObject(RequestObject);
		Request->SetHeader("Authorization", APIKey);
		Request->SetHeader("Content-Type", "application/json");
		// 보안이 필요하지만 생략
		Request->SetHeader("OpenAI-Organization", "org-BZKAwjvbzNgg3Uszu2Pb91Hz");
		{
			FScriptDelegate Delegate{};
			Delegate.BindUFunction(Target, TEXT("OnRequestComplete"));
			Request->OnRequestComplete.Add(Delegate);
		}
		{
			FScriptDelegate Delegate{};
			Delegate.BindUFunction(Target, TEXT("OnRequestFail"));
			Request->OnRequestFail.Add(Delegate);
		}
		Request->ProcessURL("https://api.openai.com/v1/responses");
	}
}

FString UC_VARest::GetStringFromRequestJSON(UVaRestRequestJSON* Request)
{
	if (!Request)
		return {};
	FString result = Request->GetResponseContentAsString();
	result.Split("\"text\":", nullptr, &result);
	result.Split("\n", &result, nullptr);
	result = result.Mid(2, result.Len() - 4);
	UE_LOG(LogTemp, Warning, TEXT("GPT Response: %d"), result.Len());
	return result;
}
