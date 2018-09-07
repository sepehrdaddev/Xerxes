#ifndef BASE_FLOOD_H
#define BASE_FLOOD_H

#include "../Headers/Attack_Vector.hpp"

class Base_Flood : Attack_Vector {
public:
	Base_Flood() = default;
	explicit Base_Flood(std::shared_ptr<Config> config);

	virtual ~Base_Flood() = default;

private:
};

#endif /* BASE_FLOOD_H */
