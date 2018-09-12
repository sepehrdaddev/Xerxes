#ifndef BASE_FLOOD_H
#define BASE_FLOOD_H

#include "../Headers/Attack_Vector.hpp"
#include "../Headers/httphdr.hpp"

class Base_Flood : Attack_Vector {
public:
	Base_Flood() = default;
	explicit Base_Flood(std::shared_ptr<Config> config);

	virtual void attack() override;

	virtual ~Base_Flood() = default;

private:
  virtual void init_header(httphdr& httph);

};

#endif /* BASE_FLOOD_H */
