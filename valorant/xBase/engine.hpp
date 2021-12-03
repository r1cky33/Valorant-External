#pragma once

#define M_PI	3.1415926535

#define UCONST_Pi           3.1415926
#define RadianToURotation 180.0f / UCONST_Pi

namespace engine {
#ifndef STRUCTS
#define STRUCTS
	struct _MYMATRIX {
		union {
			struct {
				float        _11, _12, _13, _14;
				float        _21, _22, _23, _24;
				float        _31, _32, _33, _34;
				float        _41, _42, _43, _44;

			};
			float m[4][4];
		};
	};

	struct FQuat
	{
		float x;
		float y;
		float z;
		float w;
	};

	struct FTransform
	{
		FQuat rot;
		Vector3 translation;
		char pad[4];
		Vector3 scale;
		char pad1[4];
		_MYMATRIX ToMatrixWithScale()
		{
			_MYMATRIX m;
			m._41 = translation.x;
			m._42 = translation.y;
			m._43 = translation.z;

			float x2 = rot.x + rot.x;
			float y2 = rot.y + rot.y;
			float z2 = rot.z + rot.z;

			float xx2 = rot.x * x2;
			float yy2 = rot.y * y2;
			float zz2 = rot.z * z2;
			m._11 = (1.0f - (yy2 + zz2)) * scale.x;
			m._22 = (1.0f - (xx2 + zz2)) * scale.y;
			m._33 = (1.0f - (xx2 + yy2)) * scale.z;

			float yz2 = rot.y * z2;
			float wx2 = rot.w * x2;
			m._32 = (yz2 - wx2) * scale.z;
			m._23 = (yz2 + wx2) * scale.y;

			float xy2 = rot.x * y2;
			float wz2 = rot.w * z2;
			m._21 = (xy2 - wz2) * scale.y;
			m._12 = (xy2 + wz2) * scale.x;

			float xz2 = rot.x * z2;
			float wy2 = rot.w * y2;
			m._31 = (xz2 + wy2) * scale.z;
			m._13 = (xz2 - wy2) * scale.x;

			m._14 = 0.0f;
			m._24 = 0.0f;
			m._34 = 0.0f;
			m._44 = 1.0f;

			return m;
		}
	};

	struct FMinimalViewInfo
	{
		Vector3 Location; //+ 0x1260
		Vector3 Rotation; //+ 0x126C
		float FOV;     //+ 0x1278
	};
#endif

	extern _MYMATRIX ToMatrix(Vector3 Rotation, Vector3 origin);
	extern Vector3 WorldToScreen(Vector3 world_location, Vector3 position, Vector3 rotation, float fov);
	extern _MYMATRIX MatrixMultiplication(_MYMATRIX pM1, _MYMATRIX pM2);

#ifndef BONES
#define BONES
	enum e_female_bones
	{
		f_Skeleton = 0,
		f_Root = 1,
		f_Splitter = 2,
		f_Spine1 = 3,
		f_Spine2 = 4,
		f_Spine3 = 5,
		f_Spine4 = 6,
		f_Neck = 7,
		f_Head = 8,
		f_Jaw = 9,
		f_Mouth_Bot = 10,
		f_L_EyeLids = 11,
		f_R_Eyeball = 12,
		f_Mouth_Top = 13,
		f_L_Eyeball = 14,
		f_MouthCorner = 15,
		f_R_EyeLids = 16,
		f_L_Brows = 17,
		f_R_Brows = 18,
		f_Collar = 19,
		f_L_Clavicle = 20,
		f_L_Shoulder = 21,
		f_L_Elbow = 22,
		f_L_Hand = 23,
		f_L_Pinky1 = 24,
		f_L_Pinky2 = 25,
		f_L_Pinky3 = 26,
		f_L_Thumb1 = 27,
		f_L_Thumb2 = 28,
		f_L_Thumb3 = 29,
		f_L_Index1 = 30,
		f_L_Index2 = 31,
		f_L_Index3 = 32,
		f_L_Ring1 = 33,
		f_L_Ring2 = 34,
		f_L_Ring3 = 35,
		f_L_Middle1 = 36,
		f_L_Middle2 = 37,
		f_L_Middle3 = 38,
		f_L_WeaponPoint = 39,
		f_L_Elbow_Ndl = 40,
		f_L_Elbow_Twst2 = 41,
		f_L_Elbow_Twst3 = 42,
		f_L_Shoulder_Twst1 = 43,
		f_L_Shoulder_Twst3 = 44,
		f_R_Clavicle = 45,
		f_R_Shoulder = 46,
		f_R_Elbow = 47,
		f_R_Hand = 48,
		f_R_WeaponPoint = 49,
		f_R_Index1 = 50,
		f_R_Index2 = 51,
		f_R_Index3 = 52,
		f_R_Thumb1 = 53,
		f_R_Thumb2 = 54,
		f_R_Thumb3 = 55,
		f_R_Ring1 = 56,
		f_R_Ring2 = 57,
		f_R_Ring3 = 58,
		f_R_Pinky1 = 59,
		f_R_Pinky2 = 60,
		f_R_Pinky3 = 61,
		f_R_Middle1 = 62,
		f_R_Middle2 = 63,
		f_R_Middle3 = 64,
		f_R_Elbow_Ndl = 65,
		f_R_Elbow_Twst2 = 66,
		f_R_Elbow_Twst3 = 67,
		f_R_Shoulder_Twst1 = 68,
		f_R_Shoulder_Twst3 = 69,
		f_Pelvis = 70,
		f_R_Foot_IKpv_IUE = 71,
		f_L_Foot_IKpv_IUE = 72,
		f_L_Hip = 73,
		f_L_Knee = 74,
		f_L_Knee_Ndl = 75,
		f_L_Foot = 76,
		f_L_Toe = 77,
		f_L_Hip_Twst1 = 78,
		f_L_Hip_Twst3 = 79,
		f_R_Hip = 80,
		f_R_Knee = 81,
		f_R_Knee_Ndl = 82,
		f_R_Foot = 83,
		f_R_Toe = 84,
		f_R_Hip_Twst1 = 85,
		f_R_Hip_Twst3 = 86,
		f_MasterWeaponAim = 87,
		f_MasterWeapon = 88,
		f_L_WeaponMaster = 89,
		f_R_WeaponMaster = 90,
		f_R_Weapon_HandTarget = 91,
		f_L_Weapon_HandTarget = 92,
		f_L_Weapon_HandOffset = 93,
		f_L_Hand_IKpv_IUE = 94,
		f_R_Hand_IKpv_IUE = 95,
		f_IK_RootTarget = 96,
		f_L_IK_FootTarget = 97,
		f_R_IK_FootTarget = 98
	};

	enum e_male_bones
	{
		Skeleton = 0,
		Root = 1,
		Splitter = 2,
		Spine1 = 3,
		Spine2 = 4,
		Spine3 = 5,
		Spine4 = 6,
		Neck = 7,
		Head = 8,
		Mouth_Top = 9,
		L_Brows = 10,
		R_Eyeball = 11,
		Jaw = 12,
		Mouth_Bot = 13,
		L_Eyeball = 14,
		L_EyeLids = 15,
		MouthCorner = 16,
		R_EyeLids = 17,
		R_Brows = 18,
		Collar = 19,
		L_Clavicle = 20,
		L_Shoulder = 21,
		L_Elbow = 22,
		L_Hand = 23,
		L_WeaponPoint = 24,
		L_Thumb1 = 25,
		L_Thumb2 = 26,
		L_Thumb3 = 27,
		L_Index1 = 28,
		L_Index2 = 29,
		L_Index3 = 30,
		L_Middle1 = 31,
		L_Middle2 = 32,
		L_Middle3 = 33,
		L_Pinky1 = 34,
		L_Pinky2 = 35,
		L_Pinky3 = 36,
		L_Ring1 = 37,
		L_Ring2 = 38,
		L_Ring3 = 39,
		L_Elbow_Ndl = 40,
		L_Elbow_Twst2 = 41,
		L_Elbow_Twst3 = 42,
		L_Shoulder_Twst1 = 43,
		L_Shoulder_Helper = 44,
		L_Shoulder_Twst3 = 45,
		R_Clavicle = 46,
		R_Shoulder = 47,
		R_Elbow = 48,
		R_Hand = 49,
		R_WeaponPoint = 50,
		R_Thumb1 = 51,
		R_Thumb2 = 52,
		R_Thumb3 = 53,
		R_Index1 = 54,
		R_Index2 = 55,
		R_Index3 = 56,
		R_Middle1 = 57,
		R_Middle2 = 58,
		R_Middle3 = 59,
		R_Pinky1 = 60,
		R_Pinky2 = 61,
		R_Pinky3 = 62,
		R_Ring1 = 63,
		R_Ring2 = 64,
		R_Ring3 = 65,
		R_Elbow_Ndl = 66,
		R_Elbow_Twst2 = 67,
		R_Elbow_Twst3 = 68,
		R_Shoulder_Twst1 = 69,
		R_Shoulder_Helper = 70,
		R_Shoulder_Twst3 = 71,
		Pelvis = 72,
		R_Foot_IKpv_IUE = 73,
		L_Foot_IKpv_IUE = 74,
		L_Hip = 75,
		L_Knee = 76,
		L_Knee_Ndl = 77,
		L_Foot = 78,
		L_Toe = 79,
		L_Hip_Twst1 = 80,
		L_Hip_Twst3 = 81,
		R_Hip = 82,
		R_Knee = 83,
		R_Knee_Ndl = 84,
		R_Foot = 85,
		R_Toe = 86,
		R_Hip_Twst1 = 87,
		R_Hip_Twst3 = 88,
		MasterWeaponAim = 89,
		MasterWeapon = 90,
		L_WeaponMaster = 91,
		R_WeaponMaster = 92,
		L_Weapon_HandTarget = 93,
		L_Weapon_HandOffset = 94,
		R_Weapon_HandTarget = 95,
		L_Hand_IKpv_IUE = 96,
		R_Hand_IKpv_IUE = 97,
		IK_RootTarget = 98,
		L_IK_FootTarget = 99,
		R_IK_FootTarget = 100,
		TP_Camera = 101
	};

#endif // !BONES

	extern std::list<int> m_upper_part;
	extern std::list<int> m_left_arm;
	extern std::list<int> m_right_arm;
	extern std::list<int> m_spine;
	extern std::list<int> m_lower_right;
	extern std::list<int> m_lower_left;
	extern std::list<std::list<int>> m_skeleton;

	extern std::list<int> f_upper_part;
	extern std::list<int> f_left_arm;
	extern std::list<int> f_right_arm;
	extern std::list<int> f_spine;
	extern std::list<int> f_lower_right;
	extern std::list<int> f_lower_left;
	extern std::list<std::list<int>> f_skeleton;


	extern FTransform GetBoneIndex(uintptr_t mesh, int index);
	extern Vector3 GetBoneWithRotation(uintptr_t mesh, int id);
}
