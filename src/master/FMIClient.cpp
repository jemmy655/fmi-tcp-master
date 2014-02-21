#include <fmitcp/Client.h>
#include <fmitcp/Logger.h>

#include "master/Master.h"
#include "master/FMIClient.h"

using namespace fmitcp_master;

FMIClient::FMIClient(Master* master, fmitcp::EventPump* pump) : fmitcp::Client(pump) {
    m_master = master;
    m_initialized = false;
    m_state = FMICLIENT_STATE_START;
    m_isInstantiated = false;
};

FMIClient::~FMIClient(){

};

void FMIClient::onConnect(){
    m_master->slaveConnected(this);
};

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

FMIClientState FMIClient::getState(){
    return m_state;
};

bool FMIClient::isInitialized(){
    return m_initialized;
};

void FMIClient::onGetXmlRes(int mid, string xml){
    m_xml = xml;
    m_master->onSlaveGetXML(this);
};

void FMIClient::on_fmi2_import_instantiate_res(int mid, fmitcp_proto::jm_status_enu_t status){
    m_master->onSlaveInstantiated(this);
};

void FMIClient::on_fmi2_import_initialize_slave_res(int mid, fmitcp_proto::fmi2_status_t status){
    m_initialized = true; // Todo check the status
    m_master->onSlaveInitialized(this);
};

void FMIClient::on_fmi2_import_terminate_slave_res(int mid, fmitcp_proto::fmi2_status_t status){
    m_master->onSlaveTerminated(this);
};

void FMIClient::on_fmi2_import_free_slave_instance_res(int mid){
    m_master->onSlaveFreed(this);
};

void FMIClient::on_fmi2_import_do_step_res(int mid, fmitcp_proto::fmi2_status_t status){
    m_master->onSlaveStepped(this);
};

void FMIClient::on_fmi2_import_get_version_res(int mid, string version){
    m_master->onSlaveGotVersion(this);
};

void FMIClient::on_fmi2_import_set_real_res(int mid, fmitcp_proto::fmi2_status_t status){
    m_master->onSlaveSetReal(this);
};

void FMIClient::on_fmi2_import_get_real_res(int mid, const vector<double>& values, fmitcp_proto::fmi2_status_t status){
    // Store result
    m_getRealResult = values;

    // Notify master
    m_master->onSlaveGotReal(this);
};

void FMIClient::on_fmi2_import_get_fmu_state_res(int mid, int stateId){
    m_master->onSlaveGotState(this);
};

void FMIClient::on_fmi2_import_set_fmu_state_res(int mid, fmitcp_proto::fmi2_status_t status){
    m_master->onSlaveSetState(this);
};

void FMIClient::on_fmi2_import_free_fmu_state_res(int mid, fmitcp_proto::fmi2_status_t status){
    m_master->onSlaveFreedState(this);
};

// TODO:

/*
//void on_fmi2_import_reset_slave_res                     (int mid, fmitcp_proto::fmi2_status_t status);
//void on_fmi2_import_set_real_input_derivatives_res      (int mid, fmitcp_proto::fmi2_status_t status);
//void on_fmi2_import_get_real_output_derivatives_res     (int mid, fmitcp_proto::fmi2_status_t status, const vector<double>& values);
//void on_fmi2_import_cancel_step_res                     (int mid, fmitcp_proto::fmi2_status_t status);
//void on_fmi2_import_get_status_res                      (int mid, fmitcp_proto::fmi2_status_t status);
//void on_fmi2_import_get_real_status_res                 (int mid, double value);
//void on_fmi2_import_get_integer_status_res              (int mid, int value);
//void on_fmi2_import_get_boolean_status_res              (int mid, bool value);
//void on_fmi2_import_get_string_status_res               (int mid, string value);
//void on_fmi2_import_instantiate_model_res               (int mid, fmitcp_proto::jm_status_enu_t status);
//void on_fmi2_import_free_model_instance_res             (int mid);
//void on_fmi2_import_set_time_res                        (int mid, fmitcp_proto::fmi2_status_t status);
//void on_fmi2_import_set_continuous_states_res           (int mid, fmitcp_proto::fmi2_status_t status);
//void on_fmi2_import_completed_integrator_step_res       (int mid, bool callEventUpdate, fmitcp_proto::fmi2_status_t status);
//void on_fmi2_import_initialize_model_res                (int mid, bool iterationConverged, bool stateValueReferencesChanged, bool stateValuesChanged, bool terminateSimulation, bool upcomingTimeEvent, double nextEventTime, fmitcp_proto::fmi2_status_t status);
//void on_fmi2_import_get_derivatives_res                 (int mid, const vector<double>& derivatives, fmitcp_proto::fmi2_status_t status);
//void on_fmi2_import_get_event_indicators_res            (int mid, const vector<double>& eventIndicators, fmitcp_proto::fmi2_status_t status);
//void on_fmi2_import_eventUpdate_res                     (int mid, bool iterationConverged, bool stateValueReferencesChanged, bool stateValuesChanged, bool terminateSimulation, bool upcomingTimeEvent, double nextEventTime, fmitcp_proto::fmi2_status_t status);
//void on_fmi2_import_completed_event_iteration_res       (int mid, fmitcp_proto::fmi2_status_t status);
//void on_fmi2_import_get_continuous_states_res           (int mid, const vector<double>& states, fmitcp_proto::fmi2_status_t status);
//void on_fmi2_import_get_nominal_continuous_states_res   (int mid, const vector<double>& nominal, fmitcp_proto::fmi2_status_t status);
//void on_fmi2_import_terminate_res                       (int mid, fmitcp_proto::fmi2_status_t status);
//void on_fmi2_import_set_debug_logging_res               (int mid, fmitcp_proto::fmi2_status_t status);
//void on_fmi2_import_set_integer_res                     (int mid, fmitcp_proto::fmi2_status_t status);
//void on_fmi2_import_set_boolean_res                     (int mid, fmitcp_proto::fmi2_status_t status);
//void on_fmi2_import_set_string_res                      (int mid, fmitcp_proto::fmi2_status_t status);
//void on_fmi2_import_get_integer_res                     (int mid, const vector<int>& values, fmitcp_proto::fmi2_status_t status);
//void on_fmi2_import_get_boolean_res                     (int mid, const vector<bool>& values, fmitcp_proto::fmi2_status_t status);
//void on_fmi2_import_get_string_res                      (int mid, const vector<string>& values, fmitcp_proto::fmi2_status_t status);
*/
