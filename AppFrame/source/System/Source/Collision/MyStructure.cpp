#include "../../Header/Collision/MyStructure.h"

// ‰ñ“]ˆ—
// x²->y²->z²‚Ì‡”Ô‚Å, Še²‚ğ‰ñ“]‚³‚¹‚é
void OBB::Rotate(VECTOR vRot)
{
	MATRIX m = MGetRotX(vRot.x);
	m = MMult(m, MGetRotY(vRot.y));
	m = MMult(m, MGetRotZ(vRot.z));

	for (int i = 0; i < 3; i++) {
		dir_vec[i] = VTransform(dir_vec[i], m);
	}
}

// •`‰æˆ—
void OBB::Render(unsigned int color)
{
	for (int i = 0; i < 3; i++) {
		VECTOR vDiff = VScale(dir_vec[i], length[i] / 2.0f);

		VECTOR pos1 = VAdd(pos, VScale(vDiff, -1.0f));			
		VECTOR pos2 = VAdd(pos, vDiff);

		int vecIndex1 = (i + 1) % 3;
		int vecIndex2 = (i + 2) % 3;

		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < 2; k++) {
				int vecSign1 = j == 0 ? -1 : 1;
				int vecSign2 = k == 0 ? -1 : 1;
				MATRIX m = MGetTranslate(VScale(dir_vec[vecIndex1], length[vecIndex1] / 2.0f * vecSign1));
				m = MMult(m, MGetTranslate(VScale(dir_vec[vecIndex2], length[vecIndex2] / 2.0f * vecSign2)));

				VECTOR linePos1 = VTransform(pos1, m);
				VECTOR linePos2 = VTransform(pos2, m);

				DrawLine3D(linePos1, linePos2, color);
			}
		}
	}
}

void Capsule::Render(unsigned int color)
{
	DrawCapsule3D(down_pos, up_pos, r, 16, color, color, false);
}