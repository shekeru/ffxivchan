#pragma once
#include "base.h"

struct MarketOfferings {
	struct {
		UINT64 ListingId, RetainerId, OwnerId, ArtisanId;
		UINT32 Price, Fees, Tax, Qty, CatalogId;
		UINT16 TimeOffset, _Container;
		UINT32 _Slot; UINT16 _Repair, _Bond;
		UINT16 Materia[5], _nil1; UINT32 _nil2;
		CHAR RetainerName[32], PlayerName[32];
		BYTE IsHQ, MateriaCount, OnMann, CityId;
		UINT16 StainId, _nil3; UINT32 _nil4;
	} Entries[10]; BYTE IndexEnd, 
		IndexStart; UINT16 RequestId;
};

struct MarketHistory {
	UINT32 catalogId, catalogId2;
	struct {
		UINT32 Price, Timestamp, Quantity;
		BYTE IsHQ, Materia, OnMann;
		CHAR BuyerName[33];
		UINT32 catalogId;
	} Entries[20];
};

struct TaxRates {
	BYTE _nil[8];
	UINT Lominsa, Gridania, Uldah,
		Ishgard, Kugane, Crystarium;
};