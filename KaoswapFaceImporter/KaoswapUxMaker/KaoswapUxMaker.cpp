#include <iostream>
#include <fstream>
#include "StructKaoSwap.h"

using namespace std;


char szKaoswapFileName[] = "KAOSWAP.N6P";
char szKaoswapUxFileName[] = "KAOSWAPUX.N6P";
char szKaoswapUxSilhouette[] = "KaoswapUXMaker.dat2";

KAOSWAP_TENSHOUKI<1332>	srcKaoSwap;		// KAOSWAP  ���\�z���邽�߂̃o�C�i��
KAOSWAP_TENSHOUKI<2932>	dstKaoSwapUX;	// KAOSWAPUX���\�z���邽�߂̃o�C�i��

// KaoSwapUx�p�̃V���G�b�g
struct KAOSWAP_TENSHOUKI_SILHOUETTE
{
	KAOSWAP_TENSHOUKI_FACE_64x80 face[1600];
} srcKaoSwapSilhouette;

enum ErrorType { NoErr = 0, ErrNoTargetFile = -1, ErrNoKaoSwap = -6, ErrDefaultKaoswap = -8, ErrBrokenKaoswap = -9 };

/*
* ��X���b�v�t�@�C����ǂ��srcKaoSwap�Ɋi�[����B
*/
ErrorType ReadKaoSwap() {
	fstream inKaoSwap;

	// ���ꂪ�����ƁA�Ȃ����t�@�C�����F������Ȃ����Ƃ�����c
	DWORD dwAttr = ::GetFileAttributes(szKaoswapFileName);

	if (dwAttr == 0xffffffff) {
		cout << endl << "NoExist:" << szKaoswapFileName << endl;
	}

	if (!inKaoSwap) { return ErrNoKaoSwap; }


	inKaoSwap.open(szKaoswapFileName, ios::in | ios::binary);

	// ��X���b�v�\���̂ւƃR�s�[
	inKaoSwap.read((char *)(&srcKaoSwap), sizeof(srcKaoSwap));

	// �t�@�C���T�C�Y�𒲂ׂ�
	size_t fileSize = (size_t)inKaoSwap.seekg(0, ios::end).tellg();

	cout << (int)fileSize << endl;

	// �����CD�f�t�H�B
	if (fileSize == 339854) {
		return ErrDefaultKaoswap;
	}

	// �X�g���[���̃|�C���^����ԑO �s�v�����܂��A�����������������̂Łc
	inKaoSwap.seekg(0, ios::beg);

	return NoErr;
}

/*
* ��X���b�v�p�V���G�b�g�t�@�C����ǂ��srcKaoSwapSilhouette�Ɋi�[����B
*/
ErrorType ReadKaoSwapSilhouette() {
	fstream inKaoSwap;

	// ���ꂪ�����ƁA�Ȃ����t�@�C�����F������Ȃ����Ƃ�����c
	DWORD dwAttr = ::GetFileAttributes(szKaoswapUxSilhouette);
	if (dwAttr == 0xffffffff) {
		cout << endl << "NoExist:" << szKaoswapUxSilhouette << endl;
	}

	inKaoSwap.open(szKaoswapUxSilhouette, ios::in | ios::binary);

	if (!inKaoSwap) { return ErrNoTargetFile; }

	// ��X���b�v�\���̂ւƃR�s�[
	inKaoSwap.read((char *)(&srcKaoSwapSilhouette), sizeof(srcKaoSwapSilhouette));
	return NoErr;

}

/*
* �f���o���p�\���̂ł���dstKaoSwapUX������
*/
void InitKaoswapUx() {
	// UX�̃t���O�����͑S�ĂP�Ɏd���ĂȂ���
	memset(dstKaoSwapUX.flag, 0x01, sizeof(dstKaoSwapUX.flag));

	// UX�̊���e�͑S��0xFF��Ԃɂ���B�S�Ă̊�͐^����
	memset(dstKaoSwapUX.face, 0xFF, sizeof(dstKaoSwapUX.face));
}

/*
* srcKaoWwap�\���́�dstKaoSwapUX�ւƃR�s�[�B�t���O�A��Ƃ��擪����̐l���������R�s�[
*/
void CopyKaoswapToKaoswapUx() {
	memcpy((char *)(&dstKaoSwapUX.flag), (char *)(&srcKaoSwap.flag), sizeof(srcKaoSwap.flag));
	memcpy((char *)(&dstKaoSwapUX.face), (char *)(&srcKaoSwap.face), sizeof(srcKaoSwap.face));
}

/*
* dstKaoSwapUX�̎c��̕�����srcKaoSwapSilhouette����R�s�[
*/
void CopyKaoswapSilhouetteToKaoswapUx() {
	// 0�I���W���ŁA1332�Ԉȍ~������������
	memcpy((char *)(&dstKaoSwapUX.face[1332]), (char *)(&srcKaoSwapSilhouette.face), sizeof(srcKaoSwapSilhouette.face));
}

/*
* ��X���b�v�pKaoswapUX.n6p�t�@�C����dstKaoSwapUX�\���̂��珑���o��
*/
ErrorType WriteKaoSwap() {
	fstream outKaoSwap;

	outKaoSwap.open(szKaoswapUxFileName, ios::out | ios::binary);

	if (!outKaoSwap) { return ErrNoKaoSwap; }

	// ��X���b�v�ւƏ�������
	outKaoSwap.write((char *)(&dstKaoSwapUX), sizeof(dstKaoSwapUX));
	return NoErr;

}

void main(void) {

	ErrorType iErr = ReadKaoSwap();
	if (iErr == ErrDefaultKaoswap) {
		cout << "���̃t�@�C����CD�Y�t�̂��̂Ɠ���ł��B1332�l�p�Ƃ��Ă�1932�l�p�Ƃ��Ă����p���\�ł��B�����̕K�v�͂���܂���B" << endl;
		cout << "\n���������Ă��������c\n";
		getchar();
		exit(-1);
	}
	if (iErr == ErrBrokenKaoswap) {
		cout << "����KAOSWAP.N6P�͂��łɃc�[���ŔF���s�\�ȏ�ԂƂȂ��Ă��܂��Ă��܂��B�����o���܂���B" << endl;
		cout << "\n���������Ă��������c\n";
		getchar();
		exit(-1);
	}

	cout << "KAOSWAP.N6P�̊�̉摜�𓊉e���āAKAOSWAPUX.N6P���쐬���܂��B\n";
	cout << "KAOSWAPUX.N6P �����݂����Ƃ��Ă��A�e�͂Ȃ��㏑�����܂�!!�B�o�b�N�A�b�v�Ƃ��Ƃ��Ȃ獡��!!\n";
	cout << "�蔲���c�[���ł��߂�˥��orz\n";
	cout << "\n���������Ă��������c\n";
	getchar();

	// -------------�ʏ폈���X�^�[�g---------------
	ReadKaoSwapSilhouette();

	InitKaoswapUx();

	CopyKaoswapToKaoswapUx();
	CopyKaoswapSilhouetteToKaoswapUx();

	WriteKaoSwap();
}
