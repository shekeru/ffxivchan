class Aura {
public:
	USHORT Type, Stacks;
	FLOAT Timer; UINT CastId;
};

class Recast {
public:
	int Cooldown, ActionId;
	float Elapsed, Recast;
};

class ComboArea {
	float Timer; 
	int LastSkill, 
		IsQueued,
		_v9, 
		NextSkill;
public:
	bool Is(int action, float margin = 0.f) {
		return LastSkill == action && Timer >= margin;
	};
	char Set(char success, int next) {
		if (success || !next || !NextSkill)
			return success; 
		NextSkill = next;
		return TRUE;
	};
};

class ActionSys {
public:
	// Local Data
	void* _VMT1;
	float Animation;
	char _pad[84];
	ComboArea Combo;
};