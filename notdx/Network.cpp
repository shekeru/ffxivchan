#include "user.h"
#include "Network.h"

PVOID ProcessZonePacket;
UINT64 __fastcall hkPzPacket(INT64 a1, UINT a2, INT64 a3) {
	static auto eval = (decltype(&hkPzPacket))ProcessZonePacket;
	MarketOfferings* lists = (MarketOfferings*)(a3 + 16);
	MarketHistory* hist = (MarketHistory*)(a3 + 16);
	TaxRates* tax = (TaxRates*)(a3 + 16);
	switch (*(UINT16*)(a3 + 2)) {
		//case 0x25E:
		//	printf("MarketTaxRates: %i\n", a2);
		//	break;
		//case 0x328:
		//	printf("MarketBoardItemRequestStart: %i\n", lists->RequestId);
		//	break;
		//case 0x15F:
		//	printf("MarketBoardOfferings: %i\n", lists->RequestId);
		//  printf("FUCK: %i, %i\n", lists->IndexEnd, lists->IndexStart);
		//	break;
		//case 0x113:
		//	printf("MarketBoardHistory, ItemId: %i\n", 
		//		hist->catalogId); break;
	}; return eval(a1, a2, a3);
}

void Hooks::NetworkAttach() {
	//ProcessZonePacket = (PVOID)game->GetLocation(Offsets::NETWORK);
	//DetourAttach(&ProcessZonePacket, hkPzPacket);
}