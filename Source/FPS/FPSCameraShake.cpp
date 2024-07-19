// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSCameraShake.h"

UFPSCameraShake::UFPSCameraShake()
{
	//화면 흔들림 지속시간(0미만은 무한정, 0은 징동 없음)
	OscillationDuration = 0.1f;
	//점점 진동이 커지는 시간(블렌드 인)
	OscillationBlendInTime = 0.0f;
	//점점 진동이 작아지는 신간(블렌드 아웃)
	OscillationBlendOutTime = 0.0f;
	// * LocOscillation : 위치 진동
	LocOscillation.Y.Amplitude = 2.f; //Y축 진폭
	LocOscillation.Y.Frequency = 2.f; //Y축 빈도
	LocOscillation.Z.Amplitude = 3.f; //Y축 진폭
	LocOscillation.Z.Frequency = 3.f; //Y축 빈도
}