#pragma once
#include <vector>
#include "dxlib.h"
#include "appframe.h"
class bone
{
public:
	bone(int* Model,std::vector<int> list,int size);
	~bone();
	void SetMain(VECTOR* pos_list);
	void SetBoneDir(VECTOR world_dir_vec, VECTOR boon_pos, int target_frame, int parent_frame, MATRIX trans_mat, VECTOR dir_parent);;
	static const VECTOR orign;

	VECTOR* orign_pos;

private :
	int* model;
	int list_size;
	VECTOR* vec_dir_list;
	MATRIX* trans_mat_list;
	std::vector<int> Frame_list;

};

