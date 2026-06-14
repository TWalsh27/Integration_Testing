This project is essentially the system design for the Telemetry Payload Carrier created by Hunter R. Latta and myself.

Right now, the project is primarily an integration test, with three seperate sanity checks to ensure
that all components work seemlessly.

Please note that you must be using Teensy 4.0 and AdiFruit libraries, so I found it best to be utilizing the 
Arduino IDE for testing.

* Main Integration Test:

    Currently, this file attempts to write to a CSV file (not yet implemented) in the following format:

        "time_ms,temp_C,pressure_hPa,altitude_m,velocity_mps,"
        "accel_x,accel_y,accel_z,"
        "heading,roll,pitch,"
        "gps_lat,gps_lon,gps_alt_m,gps_speed_mps"
    
    Further documentation can be found within the file in this repo, as the code is fairly self-explainatory.
    Additionally, there are several failure detections within the code to ensure we know exactly when a component
    fails to operate.

* Sanity Checks:

    Like any sanity check, all these files are are independant programs that simplify the system down to singular
    parts. For instance, the BNO sanity check only utilizes code that helps to ensure that it works as intended.

    Currently, there are three sanity checks:

        * BNO Sanity Check
        * BMP Sanity Check
        * GPS Sanity Check
    
    More are forecasted to be added as more components are integrated.

* Current Plan of Action:

    * Utilize TeleGPS
    * Add SD Read Logic
    * Radio Broadcasting
        * This will likely be the longest implementation, as it involves using radio signals to track telemetry
        data in real time

Further updates to this repo will be made somewhat regularly, the first launch is scheduled for June 20th, 2026.
