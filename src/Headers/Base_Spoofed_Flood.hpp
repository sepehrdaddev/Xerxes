#ifndef BASE_SPOOFED_FLOOD_H
#define BASE_SPOOFED_FLOOD_H

#include "../Headers/Attack_Vector.hpp"
#include "../Headers/Configuration.hpp"

class Base_Spoofed_Flood : Attack_Vector {
public:
  Base_Spoofed_Flood() = default;
  explicit Base_Spoofed_Flood(std::shared_ptr<Config> config);

  virtual void attack() override;

  virtual ~Base_Spoofed_Flood() = default;

private:
  void init_headers(struct iphdr *ip, void *hdr);
  void overwrite_headers(struct iphdr *ip, void *hdr);

};

#endif /* BASE_SPOOFED_FLOOD_H */
