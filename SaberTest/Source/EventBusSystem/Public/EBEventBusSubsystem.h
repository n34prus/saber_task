#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameplayTagContainer.h"
#include "EBEventBusSubsystem.generated.h"

USTRUCT(BlueprintType)
struct FEBEventData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Event")
	FGameplayTag Topic;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Event")
	FGameplayTag Key;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Event")
	FString Text;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Event")
	float FloatValue = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Event")
	int32 IntValue = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Event")
	bool bBoolValue = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Event")
	TObjectPtr<UObject> Source = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Event")
	TObjectPtr<UObject> Target = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Event")
	double TimeSeconds = 0.0;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FEBOnEvent, const FEBEventData&);

UCLASS()
class EVENTBUSSYSTEMMODULE_API UEBEventBusSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;
	
	template<typename UserClass>
	FDelegateHandle SubscribeObject(FGameplayTag Topic, UserClass* Object, void (UserClass::*Func)(const FEBEventData&))
	{
		static_assert(TIsDerivedFrom<UserClass, UObject>::IsDerived, "Subscribe requires UObject-derived class");
		
		check(Object);
		check(Topic.IsValid());

		FDelegateHandle Handle = GetOrCreateChannel(Topic).AddUObject(Object, Func);

		TWeakObjectPtr<UObject> WeakObj(Object);
		ObjectSubscriptions.FindOrAdd(WeakObj).Add({Topic, Handle});

		return Handle;
	}

	template<typename FunctorType>
	FDelegateHandle SubscribeLambda(FGameplayTag Topic, UObject* Object, FunctorType&& Functor)
	{
		check(Topic.IsValid());
		check(Object);

		FDelegateHandle Handle = GetOrCreateChannel(Topic).AddLambda(Forward<FunctorType>(Functor));

		TWeakObjectPtr<UObject> WeakObj(Object);
		ObjectSubscriptions.FindOrAdd(WeakObj).Add({Topic, Handle});

		return Handle;
	}
	
	void Unsubscribe(FGameplayTag Topic, FDelegateHandle Handle);

	void UnsubscribeAll(UObject* Object);

	void Publish(const FEBEventData& Event);

	void CleanupDeadObjects();

private:
	
	TMap<FGameplayTag, FEBOnEvent> Channels;
	TMap<TWeakObjectPtr<UObject>, TArray<TPair<FGameplayTag, FDelegateHandle>>> ObjectSubscriptions;
	FTimerHandle CleanupTimerHandle;
	
	FEBOnEvent& GetOrCreateChannel(FGameplayTag Topic)
	{
		return Channels.FindOrAdd(Topic);
	}
};

