#pragma once

class Serializable
{
public:
	virtual void Serialize();
	virtual void Deserialize();
};