#include "bone.h"

const VECTOR bone::orign = VGet(0, 0, 0);

bone::bone(
	int* Model,
	std::vector<int> list,
	int size
    ) :
	model(Model),
	Frame_list(list),
	list_size(size)
{
	vec_dir_list = new VECTOR[list_size];
	orign_pos = new VECTOR[list_size+2];
	trans_mat_list = new MATRIX[list_size];

	for (int i = 0; i < list_size; i++) {
		trans_mat_list[i] = MV1GetFrameLocalMatrix(*model, Frame_list[i + 1]);
		MATRIX local_mat = MV1GetFrameLocalMatrix(*model, Frame_list[i + 2]);
		vec_dir_list[i] = VTransform(orign, local_mat);

		for (int j = 0; j < 3; j++) {
			trans_mat_list[i].m[3][j] = 0.0f;
		}
		orign_pos[i+2] = MV1GetFramePosition(*model, Frame_list[i+2]);
	}
	orign_pos[0] = MV1GetFramePosition(*model, Frame_list[0]);
	orign_pos[1] = MV1GetFramePosition(*model, Frame_list[1]);
};

bone::~bone() {
	SAFE_DELETE(vec_dir_list) ;
	SAFE_DELETE(orign_pos);
	SAFE_DELETE(trans_mat_list);
	model = nullptr;
};

void bone::SetMain(VECTOR* pos_list) {
	for (int i = 0; i < list_size; i++) {
		SetBoneDir(
			pos_list[i + 1],
			pos_list[i],
			Frame_list[i + 1],
			Frame_list[i],
			trans_mat_list[i],
			vec_dir_list[i]
		);
	};
};

void bone::SetBoneDir(
	VECTOR world_dir_vec, 
	VECTOR boon_pos,
	int target_frame,
	int parent_frame, 
	MATRIX trans_mat,
	VECTOR dir_parent)
{
	MATRIX TmpMat;
	TmpMat = MV1GetFrameLocalWorldMatrix(*model, parent_frame); // 親ボーンのローカル→ワールド
	TmpMat = MInverse(TmpMat); // 逆行列を計算
	VECTOR Local_Dir_Vec = VTransform(world_dir_vec, TmpMat);//ボーンを方向のローカル座標
	VECTOR Local_Bone_Pos = VTransform(boon_pos, TmpMat);//自分の付け根のローカル座標

	//ボーンを向けたい方向
	VECTOR Local_Dir_Look = VNorm(VSub(Local_Dir_Vec, Local_Bone_Pos));

	//今の向きから次の向きへの回転行列
	MATRIX RotMat = MGetRotVec2(dir_parent, Local_Dir_Look);

	//自分の付け根が位置になるよう、平行移動
	MATRIX shift_point = trans_mat;
	shift_point.m[3][0] = Local_Bone_Pos.x;
	shift_point.m[3][1] = Local_Bone_Pos.y;
	shift_point.m[3][2] = Local_Bone_Pos.z;

	TmpMat = MMult(RotMat, shift_point);
	MV1SetFrameUserLocalMatrix(*model, target_frame, TmpMat);
}