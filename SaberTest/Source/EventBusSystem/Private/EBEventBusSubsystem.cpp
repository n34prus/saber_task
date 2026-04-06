#include "EBEventBusSubsystem.h"

void UEBEventBusSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	Channels.Reserve(16);
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(
			CleanupTimerHandle,
			this,
			&UEBEventBusSubsystem::CleanupDeadObjects,
			5.0f,
			true
		);
	}
}

void UEBEventBusSubsystem::Deinitialize()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(CleanupTimerHandle);
	}
	ObjectSubscriptions.Empty();
	Channels.Empty();
	Super::Deinitialize();
}

void UEBEventBusSubsystem::Unsubscribe(FGameplayTag Topic, FDelegateHandle Handle)
{
	if (FEBOnEvent* Channel = Channels.Find(Topic))
	{
		Channel->Remove(Handle);
	}
}

void UEBEventBusSubsystem::UnsubscribeAll(UObject* Object)
{
	if (!Object)
		return;

	TWeakObjectPtr<UObject> WeakObj(Object);

	TArray<TPair<FGameplayTag, FDelegateHandle>>* Entries = ObjectSubscriptions.Find(WeakObj);
	if (!Entries)
		return;

	for (const TPair<FGameplayTag, FDelegateHandle>& Entry : *Entries)
	{
		if (FEBOnEvent* Channel = Channels.Find(Entry.Key))
		{
			Channel->Remove(Entry.Value);
		}
	}

	ObjectSubscriptions.Remove(WeakObj);
}

void UEBEventBusSubsystem::Publish(const FEBEventData& Event)
{
	if (!Event.Topic.IsValid())
	{
		return;
	}

	if (FEBOnEvent* Channel = Channels.Find(Event.Topic))
	{
		Channel->Broadcast(Event);
	}
}

void UEBEventBusSubsystem::CleanupDeadObjects()
{
	for (auto It = ObjectSubscriptions.CreateIterator(); It; ++It)
	{
		if (!It.Key().IsValid())
		{
			for (const auto& Entry : It.Value())
			{
				if (FEBOnEvent* Channel = Channels.Find(Entry.Key))
				{
					Channel->Remove(Entry.Value);
				}
			}
			It.RemoveCurrent();
		}
	}
}
