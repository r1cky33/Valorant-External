#include "stdafx.hpp"

#define M_PI	3.1415926535

#define UCONST_Pi           3.1415926
#define RadianToURotation 180.0f / UCONST_Pi

namespace engine {
	_MYMATRIX ToMatrix(Vector3 Rotation, Vector3 origin = Vector3(0, 0, 0))
	{
		float Pitch = (Rotation.x * float(M_PI) / 180.f);
		float Yaw = (Rotation.y * float(M_PI) / 180.f);
		float Roll = (Rotation.z * float(M_PI) / 180.f);

		float SP = sinf(Pitch);
		float CP = cosf(Pitch);
		float SY = sinf(Yaw);
		float CY = cosf(Yaw);
		float SR = sinf(Roll);
		float CR = cosf(Roll);

		_MYMATRIX Matrix;
		Matrix._11 = CP * CY;
		Matrix._12 = CP * SY;
		Matrix._13 = SP;
		Matrix._14 = 0.f;

		Matrix._21 = SR * SP * CY - CR * SY;
		Matrix._22 = SR * SP * SY + CR * CY;
		Matrix._23 = -SR * CP;
		Matrix._24 = 0.f;

		Matrix._31 = -(CR * SP * CY + SR * SY);
		Matrix._32 = CY * SR - CR * SP * SY;
		Matrix._33 = CR * CP;
		Matrix._34 = 0.f;

		Matrix._41 = origin.x;
		Matrix._42 = origin.y;
		Matrix._43 = origin.z;
		Matrix._44 = 1.f;

		return Matrix;
	}

	Vector3 WorldToScreen(Vector3 world_location, Vector3 position, Vector3 rotation, float fov)
	{
		Vector3 screen_location = Vector3(0, 0, 0);

		_MYMATRIX tempMatrix = ToMatrix(rotation);

		Vector3 vAxisX, vAxisY, vAxisZ;

		vAxisX = Vector3(tempMatrix.m[0][0], tempMatrix.m[0][1], tempMatrix.m[0][2]);
		vAxisY = Vector3(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2]);
		vAxisZ = Vector3(tempMatrix.m[2][0], tempMatrix.m[2][1], tempMatrix.m[2][2]);

		Vector3 vDelta = world_location - position;
		Vector3 vTransformed = Vector3(vDelta.Dot(vAxisY), vDelta.Dot(vAxisZ), vDelta.Dot(vAxisX));

		if (vTransformed.z < 1.f)
			vTransformed.z = 1.f;

		float FovAngle = fov;
		float ScreenCenterX = globals::wnd::screen_res_width / 2.0f;
		float ScreenCenterY = globals::wnd::screen_res_height / 2.0f;

		screen_location.x = ScreenCenterX + vTransformed.x * (ScreenCenterX / tanf(FovAngle * (float)M_PI / 360.f)) / vTransformed.z;
		screen_location.y = ScreenCenterY - vTransformed.y * (ScreenCenterX / tanf(FovAngle * (float)M_PI / 360.f)) / vTransformed.z;

		return screen_location;
	}

	_MYMATRIX MatrixMultiplication(_MYMATRIX pM1, _MYMATRIX pM2)
	{
		_MYMATRIX pOut;
		pOut._11 = pM1._11 * pM2._11 + pM1._12 * pM2._21 + pM1._13 * pM2._31 + pM1._14 * pM2._41;
		pOut._12 = pM1._11 * pM2._12 + pM1._12 * pM2._22 + pM1._13 * pM2._32 + pM1._14 * pM2._42;
		pOut._13 = pM1._11 * pM2._13 + pM1._12 * pM2._23 + pM1._13 * pM2._33 + pM1._14 * pM2._43;
		pOut._14 = pM1._11 * pM2._14 + pM1._12 * pM2._24 + pM1._13 * pM2._34 + pM1._14 * pM2._44;
		pOut._21 = pM1._21 * pM2._11 + pM1._22 * pM2._21 + pM1._23 * pM2._31 + pM1._24 * pM2._41;
		pOut._22 = pM1._21 * pM2._12 + pM1._22 * pM2._22 + pM1._23 * pM2._32 + pM1._24 * pM2._42;
		pOut._23 = pM1._21 * pM2._13 + pM1._22 * pM2._23 + pM1._23 * pM2._33 + pM1._24 * pM2._43;
		pOut._24 = pM1._21 * pM2._14 + pM1._22 * pM2._24 + pM1._23 * pM2._34 + pM1._24 * pM2._44;
		pOut._31 = pM1._31 * pM2._11 + pM1._32 * pM2._21 + pM1._33 * pM2._31 + pM1._34 * pM2._41;
		pOut._32 = pM1._31 * pM2._12 + pM1._32 * pM2._22 + pM1._33 * pM2._32 + pM1._34 * pM2._42;
		pOut._33 = pM1._31 * pM2._13 + pM1._32 * pM2._23 + pM1._33 * pM2._33 + pM1._34 * pM2._43;
		pOut._34 = pM1._31 * pM2._14 + pM1._32 * pM2._24 + pM1._33 * pM2._34 + pM1._34 * pM2._44;
		pOut._41 = pM1._41 * pM2._11 + pM1._42 * pM2._21 + pM1._43 * pM2._31 + pM1._44 * pM2._41;
		pOut._42 = pM1._41 * pM2._12 + pM1._42 * pM2._22 + pM1._43 * pM2._32 + pM1._44 * pM2._42;
		pOut._43 = pM1._41 * pM2._13 + pM1._42 * pM2._23 + pM1._43 * pM2._33 + pM1._44 * pM2._43;
		pOut._44 = pM1._41 * pM2._14 + pM1._42 * pM2._24 + pM1._43 * pM2._34 + pM1._44 * pM2._44;

		return pOut;
	}

	std::list<int> m_upper_part = { e_male_bones::Neck, e_male_bones::Head };
	std::list<int> m_left_arm = { e_male_bones::Neck, e_male_bones::L_Clavicle, e_male_bones::L_Shoulder, e_male_bones::L_Elbow, e_male_bones::L_Hand };
	std::list<int> m_right_arm = { e_male_bones::Neck, e_male_bones::R_Clavicle, e_male_bones::R_Shoulder, e_male_bones::R_Elbow, e_male_bones::R_Hand };
	std::list<int> m_spine = { e_male_bones::Neck, e_male_bones::Spine2, e_male_bones::Pelvis };
	std::list<int> m_lower_right = { e_male_bones::Pelvis, e_male_bones::R_Hip, e_male_bones::R_Knee, e_male_bones::R_Foot };
	std::list<int> m_lower_left = { e_male_bones::Pelvis, e_male_bones::L_Hip, e_male_bones::L_Knee, e_male_bones::L_Foot };
	std::list<std::list<int>> m_skeleton = { m_upper_part, m_right_arm, m_left_arm, m_spine, m_lower_right, m_lower_left };

	std::list<int> f_upper_part = { e_female_bones::f_Neck, e_female_bones::f_Head };
	std::list<int> f_left_arm = { e_female_bones::f_Neck, e_female_bones::f_L_Clavicle, e_female_bones::f_L_Shoulder, e_female_bones::f_L_Elbow, e_female_bones::f_L_Hand };
	std::list<int> f_right_arm = { e_female_bones::f_Neck, e_female_bones::f_R_Clavicle, e_female_bones::f_R_Shoulder, e_female_bones::f_R_Elbow, e_female_bones::f_R_Hand };
	std::list<int> f_spine = { e_female_bones::f_Neck, e_female_bones::f_Spine2, e_female_bones::f_Pelvis };
	std::list<int> f_lower_right = { e_female_bones::f_Pelvis, e_female_bones::f_R_Hip, e_female_bones::f_R_Knee, e_female_bones::f_R_Foot };
	std::list<int> f_lower_left = { e_female_bones::f_Pelvis, e_female_bones::f_L_Hip, e_female_bones::f_L_Knee, e_female_bones::f_L_Foot };
	std::list<std::list<int>> f_skeleton = { f_upper_part, f_right_arm, f_left_arm, f_spine, f_lower_right, f_lower_left };


	FTransform GetBoneIndex(uintptr_t mesh, int index)
	{
		uintptr_t bonearray = core::read<uintptr_t>(globals::target_proc::target_process_id, mesh + 0x558);

		if (!utils::is_valid_addr(bonearray))
			bonearray = core::read<uintptr_t>(globals::target_proc::target_process_id, mesh + 0x558);

		return core::read<FTransform>(globals::target_proc::target_process_id, bonearray + (index * 0x30));
	}

	Vector3 GetBoneWithRotation(uintptr_t mesh, int id)
	{
		FTransform bone = GetBoneIndex(mesh, id);
		FTransform ComponentToWorld = core::read<FTransform>(globals::target_proc::target_process_id, mesh + 0x250);
		_MYMATRIX Matrix;
		Matrix = MatrixMultiplication(bone.ToMatrixWithScale(), ComponentToWorld.ToMatrixWithScale());
		return Vector3(Matrix._41, Matrix._42, Matrix._43);
	}
}
