#include "health.h"

static bool s_health_available;

static void health_handler(HealthEventType event, void *context) {
  main_window_update_ui();
}

void health_init() {
  s_health_available = health_service_events_subscribe(health_handler, NULL);
  if(!s_health_available) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "Health not available!");
  }
}

bool health_is_available() {
  return s_health_available;
}

int health_get_metric_sum(HealthMetric metric) {
  HealthServiceAccessibilityMask mask = health_service_metric_accessible(metric, 
    time_start_of_today(), time(NULL));
  if(mask == HealthServiceAccessibilityMaskAvailable) {
    
    // Create an array to store data
  const uint32_t max_records = 60;
  HealthMinuteData *minute_data = (HealthMinuteData*)
                                malloc(max_records * sizeof(HealthMinuteData));
  
  // Make a timestamp for 15 minutes ago and an hour before that 
  time_t end = time(NULL);// - (15 * SECONDS_PER_MINUTE);
  time_t start = end - SECONDS_PER_HOUR;
  
  // Obtain the minute-by-minute records
  uint32_t num_records = health_service_get_minute_history(minute_data, 
                                                    max_records, &start, &end);
  APP_LOG(APP_LOG_LEVEL_INFO, "num_records: %d", (int)num_records);
  
  // Print the number of steps for each minute
  for(uint32_t i = 0; i < num_records; i++) {
    APP_LOG(APP_LOG_LEVEL_INFO, "Item %d => steps: %d; vmc: %d; orientation: %d", (int)i, 
            (int)minute_data[i].steps, 
            (int)minute_data[i].vmc, 
    (int)minute_data[i].orientation);
  }
    return (int)health_service_sum_today(metric);
  } else {
    APP_LOG(APP_LOG_LEVEL_ERROR, "Data unavailable!");
    return 0;
  }
  
  
  
  
}
