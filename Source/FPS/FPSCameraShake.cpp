// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSCameraShake.h"

UFPSCameraShake::UFPSCameraShake()
{
	//ȭ�� ��鸲 ���ӽð�(0�̸��� ������, 0�� ¡�� ����)
	OscillationDuration = 0.1f;
	//���� ������ Ŀ���� �ð�(���� ��)
	OscillationBlendInTime = 0.0f;
	//���� ������ �۾����� �Ű�(���� �ƿ�)
	OscillationBlendOutTime = 0.0f;
	// * LocOscillation : ��ġ ����
	LocOscillation.Y.Amplitude = 2.f; //Y�� ����
	LocOscillation.Y.Frequency = 2.f; //Y�� ��
	LocOscillation.Z.Amplitude = 3.f; //Y�� ����
	LocOscillation.Z.Frequency = 3.f; //Y�� ��
}