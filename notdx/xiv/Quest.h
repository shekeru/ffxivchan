#pragma once
typedef struct Quest { //144 bytes
	void* unk_1; // 8
	void* unk_2; // 16
	void* unk_3; // 24
	void* unk_4; // 32
	struct {
		int type;
		int value;
	} Id; // 40
	char* c_ptr; // 48
	__int64 length; // 56
	void* unk_5; // 64
	void* unk_6; // 72
	short unk_7; // 74
	char padded[64]; // 138
	short unk_8; // 140
	int unk_9; // 144
} Quest;

namespace data {
	typedef struct Level {
		string leveName;
		int quantity;
		bool repeated;
		int itemId;
		string itemName;
	} Level;
	void from_json(const json& j, Level& p) {
		j.at("leveName").get_to(p.leveName);
		j.at("quantity").get_to(p.quantity);
		j.at("repeated").get_to(p.repeated);
		j.at("itemId").get_to(p.itemId);
		j.at("itemName").get_to(p.itemName);
	}
}