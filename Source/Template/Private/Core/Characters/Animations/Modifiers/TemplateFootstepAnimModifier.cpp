// All Rights Reserved. Copyrighted by Vinicius Cordeiro / Cerrado Game Studio. Contact: Email: vinicordeirogo@gmail.com ; Instagram: @vinicordeirogo ; Linkedin: vinicordeirox ; GitHub: Vinicius-Cordeiro


#include "Core/Characters/Animations/Modifiers/TemplateFootstepAnimModifier.h"

#include "Core/Characters/Animations/Notifies/TemplateFootstepAnimNotify.h"

void UTemplateFootstepAnimModifier::OnApply_Implementation(UAnimSequence* AnimationSequence) {
	Super::OnApply_Implementation(AnimationSequence);
	if (!AnimationSequence) return; // Don't continue if we don't have a valid animation sequence to apply the modifier to. Still crashing? BUG??!!

	int32 AnimationLastFrame { 0 };
	UAnimationBlueprintLibrary::GetNumFrames(AnimationSequence , AnimationLastFrame);

	const bool bIsValidTrack { UAnimationBlueprintLibrary::IsValidAnimNotifyTrackName(AnimationSequence,TrackName) };

	if (bIsValidTrack) {
		UAnimationBlueprintLibrary::RemoveAnimationNotifyEventsByTrack(AnimationSequence , TrackName);
		// If the track is valid we destroy it and create a new one 
		UAnimationBlueprintLibrary::RemoveAnimationNotifyTrack(AnimationSequence , TrackName);
	}

	UAnimationBlueprintLibrary::AddAnimationNotifyTrack(AnimationSequence , TrackName , TrackColor);
	int32 CurrentIndex { 0 };

	for (int32 i = 0; i < AnimationLastFrame; i++) {
		// loop through all the animation frames looking for the first and middle animation frame for placing the animation notify
		if (i == 0 || i == AnimationLastFrame / 2) {
			// get time of the frame, so we'll be able to add the animation on the correct time
			float FrameTime { 0.f };
			const int32 Frame { i };
			UAnimationBlueprintLibrary::GetTimeAtFrame(AnimationSequence , Frame , FrameTime);
			UAnimNotify* Notify { UAnimationBlueprintLibrary::AddAnimationNotifyEvent(AnimationSequence,TrackName,FrameTime,NotifyClass) };
			if (Notify)
				Notify->NotifyColor = FColor(TrackColor.R / 3.f , TrackColor.G / 3.f , TrackColor.B / 3.f); // make notify color darker 
		}
	}
}

void UTemplateFootstepAnimModifier::OnRevert_Implementation(UAnimSequence* AnimationSequence) {
	Super::OnRevert_Implementation(AnimationSequence);
	const bool bIsValidTrack { UAnimationBlueprintLibrary::IsValidAnimNotifyTrackName(AnimationSequence,TrackName) };

	if (bIsValidTrack) {
		UAnimationBlueprintLibrary::RemoveAnimationNotifyEventsByTrack(AnimationSequence , TrackName);
		// If the track is valid we destroy it and create a new one 
		UAnimationBlueprintLibrary::RemoveAnimationNotifyTrack(AnimationSequence , TrackName);
	}

}
