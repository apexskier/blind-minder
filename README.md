# Blind Minder

Part of my ongoing quest to automate my house.

Blinds open up in the morning to help wake me up and close in the evening before bed.

Most of that's controlled [via HomeKit](https://github.com/apexskier/my-homebridge). A simple
http interface is exposed, and feedback servos allow normal manual control.

## HTTP interface

### POST `/?v={val}`

Angles blinds to `val` degrees. `val` is an int between -180 and 180.

**202** success, blinds will move to specified value

**400** invalid request, extra, missing, or invalid parameters

### GET `/`

Returns a JSON blob of data with the current status of the blinds.

**200** success
fields  
- **target** _number_ Current target angle
- **current** _number_ Current angle
- **obstructed** _boolean_ The blinds have detected an obstruction
- **moving** _boolean_ If the blinds are moving to the current target angle
