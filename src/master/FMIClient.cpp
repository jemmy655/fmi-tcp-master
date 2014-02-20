#include <fmitcp/Client.h>
#include <fmitcp/Logger.h>

#include "master/Master.h"
#include "master/FMIClient.h"

using namespace fmitcp_master;

FMIClient::FMIClient(Master* master, fmitcp::EventPump* pump) : fmitcp::Client(pump) {
    m_master = master;
};

FMIClient::~FMIClient(){

};

void FMIClient::onConnect(){
    fmi2_import_do_step(0,0,0.0,0.1,true);
};

void FMIClient::on_fmi2_import_do_step_res(int message_id, fmitcp_proto::fmi2_status_t status){
    //m_master->on_fmi2_import_do_step_res(this,message_id,status);
};

void FMIClient::on_fmi2_import_instantiate_slave_res(int message_id, fmitcp_proto::jm_status_enu_t status){
}

void FMIClient::onDisconnect(){
    m_logger.log(fmitcp::Logger::LOG_DEBUG,"onDisconnect\n");
    m_master->slaveDisconnected(this);
};

void FMIClient::onError(string err){
    m_logger.log(fmitcp::Logger::LOG_DEBUG,"onError\n");
    m_master->slaveError(this);
};

int FMIClient::getId(){
    return m_id;
};

void FMIClient::setId(int id){
    m_id = id;
};