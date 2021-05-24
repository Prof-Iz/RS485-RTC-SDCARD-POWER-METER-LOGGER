# Using RS485 Protocol to read and log power meter readings from a Schneider PM 1200

## Completed

- Data Logging / writing to SD Card with time stamp
- Interfacing with the Schneider PM 1200. This was a challenge as documentation of the Modbus configuration used by the Power Meter was not readily available. The byte order of the processor and other Modbus parameters had to be found out through reading the little available documentation and through trial and error. MATLAB was used in this sense to test the parameters.

## Pending
- Merging of logging and Modbus portion
- Making of Physical Casing / 3D print to house the sensor and deploying it in the Northwing substation of the university.


## Challenges
- The Power meter is only available in the lab (not taken home with me), as such I can only work when I am there. Due to Covid restrictions I havent been able to go to the lab for a few months and the project is currently on hold since.