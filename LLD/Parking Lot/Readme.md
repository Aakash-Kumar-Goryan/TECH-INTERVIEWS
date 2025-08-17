Rough Flow:
![Rough Flow diagram](image.png)

Requirements Clarifications:
How many Entrances?
Ans: one entrences and one exit 
Keep in mind should bew scaleable
what are different types of SPOT?
Two wheeler
Four wheeler
-- should be exitensible

Q. Hourly based charge or minute based charge?
A. Mixture based charge

Q. Do we have multi floor parking spot?
A No. but consider??
Follow up: Provide nearest PArking Spot from entry gate 


Objects:
- Vehicle 
    - Vehicle number
    - vehicle Type
        - Enum (TWO_WHEELER, FOUR_WHEELER)
- Ticket
    - Entry Time
    - Parking Spot
- Entrance Gate
    - findParkingSpace
    - updateParkingSpace
    - generateTicket
- Parking SPOT
    - id
    - isEmpty
    - getVehicle
    - Price
    - Type
- Exit Gate
    - costCalculation
    - payment
    - updateParkingSpot

2 Approaches
- Top-Down 
    - (make whatever parent need later)
- Bottom up 
    - (make low level clasess first then parent which uses them)