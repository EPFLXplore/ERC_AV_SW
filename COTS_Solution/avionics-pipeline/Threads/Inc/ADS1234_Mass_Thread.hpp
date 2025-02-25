 #ifndef ADS1234_MASS_THREAD_HPP_
 #define ADS1234_MASS_THREAD_HPP_
 
 
 #include <ADS1234.hpp>
 #include <DataStructures.h>
 #include <Prober.h>
 #include <Thread.h>
 
 #define ADS1234_HAT_ADDR (0x4E)
 
 #define USE_LOW_PASS_FILTER
 
 // For plotting with SWV Data Trace Timeline Graph
 //#define PLOT_CH1
 //#define PLOT_CH2
 //#define PLOT_CH3
 //#define PLOT_CH4
 
 
 class ADS1234Thread : public Thread {
 public:
     ADS1234Thread(ProberThread* parent, SPI_HandleTypeDef* hspi_);
     ~ADS1234Thread();

     void init();
     void loop();

     uint8_t getPortNum();
     ADS1234* get_sensor();

     static void handle_set_config(uint8_t sender_id, MassConfigPacket* packet);
     static void handle_mass_calib(uint8_t sender_id, MassCalibPacket* packet);
     static void handle_imu_calib(uint8_t sender_id, ImuCalibPacket* packet);
 
     void set_channels_status(bool state[4]);
     const bool* get_channels_status() const;
 
     void set_alpha(float alpha_);
     float get_alpha();
 
     void start_calib_offset(uint32_t num_samples, uint8_t channel);
     void start_calib_scale(uint32_t num_samples, uint8_t channel, float calib_weight);
 
     bool configured = false;
 
 private:
     ProberThread* parent;
     uint8_t portNum;
     SPI_HandleTypeDef* hspi;

     ADS1234* mass_sensor;
     
     bool enabled_channels[4] = {false, true, false, false};
     float alpha = 0.8;
     uint16_t  num_averages = 10;
     long unsigned int start = 0;
     bool calibrating = true;
 
     long unsigned int config_time = 0;
     long unsigned int config_req_interval = 5000;
 
     void request_config();
 
     // Calibration parameters
 
     uint8_t sender_id;
     uint8_t calib_channel = 1;
 
     bool calibrating_offset = false;;
     bool calibrating_scale = false;
 
     uint32_t cnt_mass_offset = 0;
     uint32_t cnt_mass_scale = 0;
 
     uint32_t calib_samples_offset = 50;
     uint32_t calib_samples_scale = 50;
 
     float calib_weight = 0;
 
     float mass_avg_offset[4];
     double mass_sum_offset[4];
 
     float mass_avg_scale[4];
     double mass_sum_scale[4];
 
     void send_calib_offset();
     void send_calib_scale();
 
     void set_sender_id(uint8_t sender_id);
 };
 
 extern ADS1234Thread* MassSensorInstance;
 
 
 
 
 #endif /* SENSORS_ADS1234_MASS_THREAD_HPP_ */
 