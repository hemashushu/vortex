#pragma once

#include "VVX_cache.h"
#include "VVX_cache__Syms.h"
#include "verilated.h"

//#ifdef VCD_OUTPUT
#include <verilated_vcd_c.h>
//#endif

//#include <VX_config.h>
#include "ram.h"
#include <ostream>
#include <vector>
#include <queue>

#define ENABLE_DRAM_STALLS
#define DRAM_LATENCY 100
#define DRAM_RQ_SIZE 16
#define DRAM_STALLS_MODULO 16
#define GLOBAL_BLOCK_SIZE 16

typedef struct {
  int cycles_left;
  uint8_t *data;
  unsigned tag;
} dram_req_t;

typedef struct {
  char valid;
  char rw; 
  unsigned byteen;
  unsigned *addr;
  unsigned *data;
  unsigned int tag;
} core_req_t;

class CacheSim {
public:
  
  CacheSim();
  virtual ~CacheSim();
  
  bool busy();  

  void reset();
  void step();
  void wait(uint32_t cycles); 
  void attach_ram(RAM* ram);

  void run();  //run until all reqs are empty
  void clear_req(); 
  void send_req(core_req_t *req);
  bool assert_equal(unsigned int* data, unsigned int tag);
  //void time_analyisis 

  //display funcs

  void get_dram_req();
  void get_core_rsp(unsigned int (&rsp)[4]);
  void get_core_req();
  bool get_core_req_ready();
  bool get_core_rsp_ready();
  void get_dram_rsp();


private:  

  void eval();  

  void eval_reqs(); 
  void eval_rsps();
  void eval_dram_bus();
  
  std::queue<core_req_t*> core_req_vec_; 
  std::vector<dram_req_t> dram_rsp_vec_;
  std::map<unsigned int, unsigned int*> core_rsp_vec_;
  int dram_rsp_active_;

  uint32_t snp_req_active_;
  uint32_t snp_req_size_;
  uint32_t pending_snp_reqs_;

  VVX_cache *cache_;
  RAM *ram_;
//#ifdef VCD_OUTPUT
  VerilatedVcdC *trace_;
//#endif
};