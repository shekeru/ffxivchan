#pragma once

template <typename T>
struct iGenericT {
	int Count; 
	T Items[100];
};

struct iSearchResult {
	int Price, Qty, HQ, 
		City, Materia, Padding;
};

typedef struct iGenericT
<iSearchResult> IntResults;

template <typename T>
struct aGenericT {
	char *_NIL, *Hits;
	T Items[100];
};

struct aSearchResult {
	char *Price, *Total, 
		*Qty, *City, *Retainer;
};

typedef struct aGenericT
	<aSearchResult> StrResults;

template <typename T>
struct oGenericT {
	char *_NIL, *Hits;
	T Items[100];
};

struct oSearchResult {
};

typedef struct oGenericT
	<oSearchResult> ObjResults;