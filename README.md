# Blind Minder

Part of my ongoing quest to automate my house.

Blinds open up in the morning to help wake me up and close in the evening before bed.

Most of that's controlled [via HomeKit](https://github.com/apexskier/my-homebridge). A simple
http interface is exposed, and feedback servos allow normal manual control.

## [Parts list](https://www.adafruit.com/wishlists?wid=417600)

## HTTP interface

### POST `/?v={val}`

Angles blinds to `val` degrees. `val` is an int between -180 and 180.

**202** success, blinds will move to specified value

**400** invalid request, extra, missing, or invalid parameters

### GET `/`

Returns a JSON blob of data with the current status of the blinds.

**200** success

JSON fields
- **target** _int_ Current target angle
- **current** _int_ Current angle
- **obstructed** _boolean_ The blinds have detected an obstruction
- **moving** _boolean_ If the blinds are moving to the current target angle
- **luminance** _float_ Luminance in lux, intended to be outside luminance
