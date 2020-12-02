///////////////////////////////////////////////////////////////////////
//  Copyright 2015 Samsung Austin Semiconductor, LLC.                //
///////////////////////////////////////////////////////////////////////

//Description : Main file for CBP2016 

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <map>
using namespace std;

#include "utils.h"
//#include "bt9.h"
#include "bt9_reader.h"
//#include "predictor.cc"
#include "predictor.h"
#include "icache.h"
#define COUNTER     unsigned long long

void CheckHeartBeat(UINT64 numIter, UINT64 numMispred, Icache *inst_cache)
{
  UINT64 dotInterval=1000000;
  UINT64 lineInterval=30*dotInterval;

 UINT64 d1K   =1000;
 UINT64 d10K  =10000;
 UINT64 d100K =100000;
 UINT64 d1M   =1000000; 
 UINT64 d10M  =10000000;
 UINT64 d30M  =30000000;
 UINT64 d60M  =60000000;
 UINT64 d100M =100000000;
 UINT64 d300M =300000000;
 UINT64 d600M =600000000;
 UINT64 d1B   =1000000000;
 UINT64 d10B  =10000000000;
  
  if(numIter == d1K){ //prints MPKI after 100K branches
    printf("  MPKBr_1K         \t : %10.4f",   1000.0*(double)(numMispred)/(double)(numIter)); 
    inst_cache->printSimpleStats();     
    fflush(stdout);
  }

  if(numIter == d10K){ //prints MPKI after 100K branches
    printf("  MPKBr_10K         \t : %10.4f",   1000.0*(double)(numMispred)/(double)(numIter));   
    inst_cache->printSimpleStats();
    fflush(stdout);
  }
  
  if(numIter == d100K){ //prints MPKI after 100K branches
    printf("  MPKBr_100K         \t : %10.4f",   1000.0*(double)(numMispred)/(double)(numIter));  
    inst_cache->printSimpleStats(); 
    fflush(stdout);
  }
  if(numIter == d1M){
    printf("  MPKBr_1M         \t : %10.4f",   1000.0*(double)(numMispred)/(double)(numIter)); 
    inst_cache->printSimpleStats();
    fflush(stdout);
  }

  if(numIter == d10M){ //prints MPKI after 100K branches
    printf("  MPKBr_10M         \t : %10.4f",   1000.0*(double)(numMispred)/(double)(numIter));   
    inst_cache->printSimpleStats();
    fflush(stdout);
  }

  if(numIter == d30M){ //prints MPKI after 100K branches
    printf("  MPKBr_30M         \t : %10.4f",   1000.0*(double)(numMispred)/(double)(numIter));  
    inst_cache->printSimpleStats(); 
    fflush(stdout);
  }

  if(numIter == d60M){ //prints MPKI after 100K branches
    printf("  MPKBr_60M         \t : %10.4f",   1000.0*(double)(numMispred)/(double)(numIter));  
    inst_cache->printSimpleStats(); 
    fflush(stdout);
  }

  if(numIter == d100M){ //prints MPKI after 100K branches
    printf("  MPKBr_100M         \t : %10.4f",   1000.0*(double)(numMispred)/(double)(numIter)); 
    inst_cache->printSimpleStats();  
    fflush(stdout);
  }
  
  if(numIter == d300M){ //prints MPKI after 100K branches
    printf("  MPKBr_300M         \t : %10.4f",   1000.0*(double)(numMispred)/(double)(numIter));  
    inst_cache->printSimpleStats(); 
    fflush(stdout);
  }

  if(numIter == d600M){ //prints MPKI after 100K branches
    printf("  MPKBr_600M         \t : %10.4f",   1000.0*(double)(numMispred)/(double)(numIter));   
    inst_cache->printSimpleStats();
    fflush(stdout);
  }

  if(numIter == d1B){ //prints MPKI after 100K branches
    printf("  MPKBr_1B         \t : %10.4f",   1000.0*(double)(numMispred)/(double)(numIter));   
    inst_cache->printSimpleStats();
    fflush(stdout);
  }
  
  if(numIter == d10B){ //prints MPKI after 100K branches
    printf("  MPKBr_10B         \t : %10.4f",   1000.0*(double)(numMispred)/(double)(numIter));   
    inst_cache->printSimpleStats();
    fflush(stdout);
  }   

}//void CheckHeartBeat

// usage: predictor <trace>

int main(int argc, char* argv[]){
  
  if (argc != 3) {
    printf("usage: %s <trace> <policy>\n", argv[0]);
    exit(-1);
  }
  
  ///////////////////////////////////////////////
  // Init variables
  ///////////////////////////////////////////////
    
    PREDICTOR  *brpred = new PREDICTOR();  // this instantiates the predictor code
    int tttt = 64* (int) pow(2,10);
    
    //Icache * inst_cache = new Icache(tttt, 8, 16, argv[2]);
    Icache * inst_cache = new Icache(tttt, 8, 64, argv[2]);
    inst_cache->checkprint();
    // delete(inst_cache);

  ///////////////////////////////////////////////
  // read each trace recrod, simulate until done
  ///////////////////////////////////////////////

    std::string trace_path;
    trace_path = argv[1];
    bt9::BT9Reader bt9_reader(trace_path);

    std::string key = "total_instruction_count:";
    std::string value;
    bt9_reader.header.getFieldValueStr(key, value);
    UINT64     total_instruction_counter = std::stoull(value, nullptr, 0);
    UINT64 current_instruction_counter = 0;
    key = "branch_instruction_count:";
    bt9_reader.header.getFieldValueStr(key, value);
    UINT64     branch_instruction_counter = std::stoull(value, nullptr, 0);
    UINT64     numMispred =0;  

    UINT64 cond_branch_instruction_counter=0;

    UINT64 uncond_branch_instruction_counter=0;
   
  ///////////////////////////////////////////////
  // read each trace record, simulate until done
  ///////////////////////////////////////////////

      OpType opType;
      UINT64 PC, PhysAddr;
      bool branchTaken;
      UINT64 branchTarget;
      UINT64 numIter = 0;

      for (auto it = bt9_reader.begin(); it != bt9_reader.end(); ++it) {
        CheckHeartBeat(++numIter, numMispred, inst_cache); //Here numIter will be equal to number of branches read

        try {
          bt9::BrClass br_class = it->getSrcNode()->brClass();

          opType = OPTYPE_ERROR; 

          if ((br_class.type == bt9::BrClass::Type::UNKNOWN) && (it->getSrcNode()->brNodeIndex())) { //only fault if it isn't the first node in the graph (fake branch)
            opType = OPTYPE_ERROR; //sanity check
          }
          else if (br_class.type == bt9::BrClass::Type::RET) {
            if (br_class.conditionality == bt9::BrClass::Conditionality::CONDITIONAL)
              opType = OPTYPE_RET_COND;
            else if (br_class.conditionality == bt9::BrClass::Conditionality::UNCONDITIONAL)
              opType = OPTYPE_RET_UNCOND;
            else {
              opType = OPTYPE_ERROR;
            }
          }
          else if (br_class.directness == bt9::BrClass::Directness::INDIRECT) {
            if (br_class.type == bt9::BrClass::Type::CALL) {
              if (br_class.conditionality == bt9::BrClass::Conditionality::CONDITIONAL)
                opType = OPTYPE_CALL_INDIRECT_COND;
              else if (br_class.conditionality == bt9::BrClass::Conditionality::UNCONDITIONAL)
                opType = OPTYPE_CALL_INDIRECT_UNCOND;
              else {
                opType = OPTYPE_ERROR;
              }
            }
            else if (br_class.type == bt9::BrClass::Type::JMP) {
              if (br_class.conditionality == bt9::BrClass::Conditionality::CONDITIONAL)
                opType = OPTYPE_JMP_INDIRECT_COND;
              else if (br_class.conditionality == bt9::BrClass::Conditionality::UNCONDITIONAL)
                opType = OPTYPE_JMP_INDIRECT_UNCOND;
              else {
                opType = OPTYPE_ERROR;
              }
            }
            else {
              opType = OPTYPE_ERROR;
            }
          }
          else if (br_class.directness == bt9::BrClass::Directness::DIRECT) {
            if (br_class.type == bt9::BrClass::Type::CALL) {
              if (br_class.conditionality == bt9::BrClass::Conditionality::CONDITIONAL) {
                opType = OPTYPE_CALL_DIRECT_COND;
              }
              else if (br_class.conditionality == bt9::BrClass::Conditionality::UNCONDITIONAL) {
                opType = OPTYPE_CALL_DIRECT_UNCOND;
              }
              else {
                opType = OPTYPE_ERROR;
              }
            }
            else if (br_class.type == bt9::BrClass::Type::JMP) {
              if (br_class.conditionality == bt9::BrClass::Conditionality::CONDITIONAL) {
                opType = OPTYPE_JMP_DIRECT_COND;
              }
              else if (br_class.conditionality == bt9::BrClass::Conditionality::UNCONDITIONAL) {
                opType = OPTYPE_JMP_DIRECT_UNCOND;
              }
              else {
                opType = OPTYPE_ERROR;
              }
            }
            else {
              opType = OPTYPE_ERROR;
            }
          }
          else {
            opType = OPTYPE_ERROR;
          }

  
          PC = it->getSrcNode()->brVirtualAddr();
          PhysAddr = it->getSrcNode()->brPhyAddr();
          branchTaken = it->getEdge()->isTakenPath();
          branchTarget = it->getEdge()->brVirtualTarget();
          inst_cache->Iaccess(PC, PC);
          
/************************************************************************************************************/

          if (opType == OPTYPE_ERROR) { 
            if (it->getSrcNode()->brNodeIndex()) { //only fault if it isn't the first node in the graph (fake branch)
              fprintf(stderr, "OPTYPE_ERROR\n");
              printf("OPTYPE_ERROR\n");
              exit(-1); //this should never happen, if it does please email CBP org chair.
            }
          }
          else if (br_class.conditionality == bt9::BrClass::Conditionality::CONDITIONAL) { //JD2_17_2016 call UpdatePredictor() for all branches that decode as conditional
            //printf("COND ");

            bool predDir = false;

            predDir = brpred->GetPrediction(PC);
            brpred->UpdatePredictor(PC, opType, branchTaken, predDir, branchTarget); 

            if(predDir != branchTaken){
              numMispred++; // update mispred stats
            }
            cond_branch_instruction_counter++;

          }
          else if (br_class.conditionality == bt9::BrClass::Conditionality::UNCONDITIONAL) { // for predictors that want to track unconditional branches
            uncond_branch_instruction_counter++;
            brpred->TrackOtherInst(PC, opType, branchTaken, branchTarget);
          }
          else {
            fprintf(stderr, "CONDITIONALITY ERROR\n");
            printf("CONDITIONALITY ERROR\n");
            exit(-1); //this should never happen, if it does please email CBP org chair.
          }

/************************************************************************************************************/
        }
        catch (const std::out_of_range & ex) {
          std::cout << ex.what() << '\n';
          break;
        }
      
      } 

    ///////////////////////////////////////////
    //print_stats
    ///////////////////////////////////////////

      printf("  TRACE \t : %s" , trace_path.c_str()); 
      printf("  NUM_INSTRUCTIONS            \t : %10llu",   total_instruction_counter);
      printf("  NUM_BR                      \t : %10llu",   branch_instruction_counter-1); //JD2_2_2016 NOTE there is a dummy branch at the beginning of the trace...
      printf("  NUM_UNCOND_BR               \t : %10llu",   uncond_branch_instruction_counter);
      printf("  NUM_CONDITIONAL_BR          \t : %10llu",   cond_branch_instruction_counter);
      printf("  NUM_MISPREDICTIONS          \t : %10llu",   numMispred);
      printf("  MISPRED_PER_1K_INST         \t : %10.4f",   1000.0*(double)(numMispred)/(double)(total_instruction_counter));
      printf("\n");
      printf("**********\n");
      inst_cache->printSimpleStats();
      fflush(stdout);
      delete(inst_cache);
      printf("\n");
}



