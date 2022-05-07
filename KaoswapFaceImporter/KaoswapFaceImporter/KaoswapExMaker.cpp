#include <iostream>
#include <fstream>
#include "StructKaoSwap.h"

using namespace std;


char szKaoswapFileName[]		= "KAOSWAP.N6P";
char szKaoswapExFileName[]		= "KAOSWAPEX.N6P";
char szKaoswapExSilhouette[]	= "KaoswapEXMaker.dat2";

KAOSWAP_TENSHOUKI<1332>	srcKaoSwap;		// KAOSWAP  を構築するためのバイナリ
KAOSWAP_TENSHOUKI<1932>	dstKaoSwapEX;	// KAOSWAPEXを構築するためのバイナリ

// KaoSwapEx用のシルエット
struct KAOSWAP_TENSHOUKI_SILHOUETTE
{
	KAOSWAP_TENSHOUKI_FACE_64x80 face[600];
} srcKaoSwapSilhouette;

enum ErrorType { NoErr=0,  ErrNoTargetFile=-1,ErrNoKaoSwap=-6, ErrDefaultKaoswap=-8, ErrBrokenKaoswap=-9 };

/*
 * 顔スワップファイルを読んでsrcKaoSwapに格納する。
 */
ErrorType ReadKaoSwap() {
	fstream inKaoSwap;

	// これが無いと、なぜかファイルが認識されないことがある…
	DWORD dwAttr = ::GetFileAttributes(szKaoswapFileName);

	if(dwAttr == 0xffffffff) {
		cout << endl << "NoExist:" << szKaoswapFileName << endl;
	}

	if (!inKaoSwap) { return ErrNoKaoSwap; }


	inKaoSwap.open(szKaoswapFileName, ios::in|ios::binary);

	// 顔スワップ構造体へとコピー
	inKaoSwap.read( (char *)(&srcKaoSwap), sizeof(srcKaoSwap) );

	// ファイルサイズを調べる
	size_t fileSize = (size_t)inKaoSwap.seekg(0, ios::end).tellg();

	cout << (int)fileSize << endl;

	// これはCDデフォ。
	if ( fileSize == 339854 ) {
		return ErrDefaultKaoswap;
	}

	// ストリームのポインタを一番前 不要だがまぁ、続きかくかもしらんので…
	inKaoSwap.seekg(0, ios::beg);

	return NoErr;
}

/*
 * 顔スワップ用シルエットファイルを読んでsrcKaoSwapSilhouetteに格納する。
 */
ErrorType ReadKaoSwapSilhouette() {
	fstream inKaoSwap;

	// これが無いと、なぜかファイルが認識されないことがある…
	DWORD dwAttr = ::GetFileAttributes(szKaoswapExSilhouette);
	if(dwAttr == 0xffffffff) {
		cout << endl << "NoExist:" << szKaoswapExSilhouette << endl;
	}

	inKaoSwap.open(szKaoswapExSilhouette, ios::in|ios::binary);

	if (!inKaoSwap) { return ErrNoTargetFile; }

	// 顔スワップ構造体へとコピー
	inKaoSwap.read( (char *)(&srcKaoSwapSilhouette), sizeof(srcKaoSwapSilhouette) );
	return NoErr;

}

/*
 * 吐き出し用構造体であるdstKaoSwapEX初期化
 */
void InitKaoswapEx() {
	// EXのフラグ部分は全て１に仕立てなおす
	memset( dstKaoSwapEX.flag, 0x01, sizeof(dstKaoSwapEX.flag) );

	// EXの顔内容は全て0xFF状態にする。全ての顔は真っ白
	memset( dstKaoSwapEX.face, 0xFF, sizeof(dstKaoSwapEX.face) );
}

/*
 * srcKaoWwap構造体→dstKaoSwapEXへとコピー。フラグ、顔とも先頭からの人数分だけコピー
 */
void CopyKaoswapToKaoswapEx() {
	memcpy((char *)(&dstKaoSwapEX.flag), (char *)(&srcKaoSwap.flag), sizeof(srcKaoSwap.flag) );
	memcpy((char *)(&dstKaoSwapEX.face), (char *)(&srcKaoSwap.face), sizeof(srcKaoSwap.face) );
}

/*
 * dstKaoSwapEXの残りの部分をsrcKaoSwapSilhouetteからコピー
 */
void CopyKaoswapSilhouetteToKaoswapEx() {
	// 0オリジンで、1332番以降を書き換える
	memcpy((char *)(&dstKaoSwapEX.face[1332]), (char *)(&srcKaoSwapSilhouette.face), sizeof(srcKaoSwapSilhouette.face) );
}

/*
 * 顔スワップ用KaoswapEX.n6pファイルをdstKaoSwapEX構造体から書き出す
 */
ErrorType WriteKaoSwap() {
	fstream outKaoSwap;

	outKaoSwap.open(szKaoswapExFileName, ios::out|ios::binary);

	if (!outKaoSwap) { return ErrNoKaoSwap; }

	// 顔スワップへと書き込み
	outKaoSwap.write( (char *)(&dstKaoSwapEX), sizeof(dstKaoSwapEX) );
	return NoErr;

}

void main(void) {

	ErrorType iErr = ReadKaoSwap();
	if ( iErr == ErrDefaultKaoswap ) {
		cout <<  "このファイルはCD添付のものと同一です。1332人用としても1932人用としても利用が可能です。処理の必要はありません。" << endl;
		cout << "\n何か押してください…\n";
		getchar();
		exit(-1);
	}
	if ( iErr == ErrBrokenKaoswap ) {
		cout << "このKAOSWAP.N6Pはすでにツールで認識不可能な状態となってしまっています。処理出来ません。" << endl;
		cout << "\n何か押してください…\n";
		getchar();
		exit(-1);
	}

	cout <<  "KAOSWAP.N6Pの顔の画像を投影して、KAOSWAPEX.N6Pを作成します。\n";
	cout <<  "KAOSWAPEX.N6P が存在したとしても、容赦なく上書きします!!。バックアップとっとくなら今だ!!\n";
	cout <<  "手抜きツールでごめんね･･･orz\n";
	cout <<  "\n何か押してください…\n";
	getchar();

	// -------------通常処理スタート---------------
	ReadKaoSwapSilhouette();

	InitKaoswapEx();

	CopyKaoswapToKaoswapEx();
	CopyKaoswapSilhouetteToKaoswapEx();

	WriteKaoSwap();
}

