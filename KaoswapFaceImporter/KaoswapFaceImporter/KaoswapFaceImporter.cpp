#include <iostream>

#include "BitMapToKaoSwap.h"

using namespace std;

template <int TNUM>
void doKaoswapImport(char *szTargetKaoSwapName) {

	auto *pBmpToKS = new BitMapToKaoSwapManager<TNUM>(szTargetKaoSwapName);

	int Err = pBmpToKS->ReadKaoSwap();
	if (!Err) {
		try {
			for (int i = 1; i <= TNUM; i++) {
				pBmpToKS->Execute(i);
			}
			pBmpToKS->WriteKaoSwap();
		}
		catch (...) { }
	}

	delete pBmpToKS;
}

void main(void) {
	doKaoswapImport<1332>("KAOSWAP.N6P");
	doKaoswapImport<1932>("KAOSWAPEX.N6P");
	doKaoswapImport<2932>("KAOSWAPUX.N6P");
}

