# DeskWatch Firmware

Arduino firmware for a desk clock. It shows the time on an OLED display, and a dedicated client on the host sends that time over the serial port. The device does not keep time itself.

## Hardware

- Arduino Nano (ATmega328P).
- 128×32 SSD1306 OLED display on I²C, at address `0x3C`, connected to the Nano's SDA (`A4`) and SCL (`A5`) pins.

## Building

The firmware needs the Arduino AVR Boards core and these libraries:

- Adafruit SSD1306
- Adafruit GFX Library
- Adafruit BusIO

To compile and upload with `arduino-cli`, replacing `<port>` with the board's serial port:

```bash
arduino-cli compile --fqbn arduino:avr:nano src/DeskWatch.Firmware
```

```bash
arduino-cli upload --fqbn arduino:avr:nano --port <port> src/DeskWatch.Firmware
```

If the upload fails on a Nano with the old bootloader, which is common on clones, use `arduino:avr:nano:cpu=atmega328old` as the board in both commands.

The sketch can also be opened in the Arduino IDE from `src/DeskWatch.Firmware/DeskWatch.Firmware.ino`.

## Serial protocol

The connection runs at 9600 baud. Each command is one line ending in `\n`, and the parameter is separated from the command by a space. Commands are case-sensitive.

Do not send `\r`: the firmware does not strip it, so a line ending in `\r\n` is rejected.

### Commands

| Command | Accepted in | Result |
|---|---|---|
| `CONNECT` | `Offline`, `Connected` | Moves to `Connected` and shows `READY`. |
| `DISCONNECT` | `Connected` | Moves to `Offline` and shows `OFFLN`. |
| `SLEEP` | `Offline` | Moves to `Sleeping` and turns the display off. |
| `WAKEUP` | `Sleeping` | Moves to `Offline`, turns the display on and shows `OFFLN`. |
| `SET <text>` | `Connected` | Shows `<text>` on the display. |

`SET` takes 1 to 5 characters, which is what fits on the display, and the value may contain spaces. For example, `SET 12:34` and `SET 12 34` can be alternated to blink the colon.

### States

| State | Meaning |
|---|---|
| `NotStarted` | Initialization has not finished or the display failed to start. Every command is rejected. |
| `Offline` | No client is connected. |
| `Connected` | A client is connected and can set the time. |
| `Sleeping` | The display is off. The microcontroller keeps running, so `WAKEUP` still works over USB. |

### Connection timeout

If the device is `Connected` and receives no recognized command for 10 seconds, it logs `connection.lost`, moves to `Offline` and shows `OFFLN`. There is no separate heartbeat command: the client sends `SET` about once a second, which also keeps the connection alive. Rejected commands reset the timeout too, but unknown commands do not.

### Starting a session

Opening the serial port normally restarts the Nano. The client should wait for the `device.started` log line before sending `CONNECT`.

## Logs

The firmware writes one JSON object per line to the serial port:

```json
{"timestamp":1234,"level":"INF","event":"connection.established","message":"Connection established"}
```

| Field | Meaning |
|---|---|
| `timestamp` | Milliseconds since the device started. |
| `level` | `INF` or `ERR`. |
| `event` | Stable identifier that the client can match on. |
| `message` | Human-readable description. |

Values are not JSON-escaped. The `command.unknown` message repeats the received command, so a command containing `"` or `\r` produces an invalid line.

### Events

| Event | Level | When |
|---|---|---|
| `device.started` | `INF` | Initialization finished. |
| `device.init.failed` | `ERR` | The display failed to start. The device stays in `NotStarted`. |
| `connection.established` | `INF` | `CONNECT` succeeded. |
| `connection.closed` | `INF` | `DISCONNECT` succeeded. |
| `connection.lost` | `ERR` | The connection timed out. |
| `connection.command.rejected` | `ERR` | `CONNECT` is not allowed in the current state. |
| `disconnection.command.rejected` | `ERR` | `DISCONNECT` is not allowed in the current state. |
| `device.asleep` | `INF` | `SLEEP` succeeded. |
| `sleep.command.rejected` | `ERR` | `SLEEP` is not allowed in the current state. |
| `device.wokeup` | `INF` | `WAKEUP` succeeded. |
| `wakeup.command.rejected` | `ERR` | `WAKEUP` is not allowed in the current state. |
| `time.synchronized` | `INF` | `SET` succeeded. |
| `set.command.rejected` | `ERR` | `SET` is not allowed in the current state, or its value is empty or too long. |
| `command.unknown` | `ERR` | The command was not recognized. |
