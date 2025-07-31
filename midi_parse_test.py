from dataclasses import dataclass
from collections.abc import Callable

#############################################
####### NOTE: This file is just for #########
####### testing puposes, plz don't  #########
####### judge my mess               #########
#############################################

# Midi constants
MIDI_HEADER_START = b"MThd"
MIDI_TRACK_START = b"MTrk"

# MIDI events
MIDI_EVT_NOTE_OFF = 0x08
MIDI_EVT_NOTE_ON  = 0x09
MIDI_EVT_POLY_PRESS = 0x0A
MIDI_EVT_CONTROLLER = 0x0B
MIDI_EVT_PROG_CHNG = 0x0C
MIDI_EVT_CHANNEL_PRESSURE = 0x0D
MIDI_EVT_PITCH_BEND = 0x0E

MIDI_EVT_NOTE_OFF_LEN = 3
MIDI_EVT_NOTE_ON_LEN = 3
MIDI_EVT_POLY_PRESS_LEN = 3
MIDI_EVT_CONTROLLER_LEN = 3
MIDI_EVT_PROG_CHNG_LEN = 2
MIDI_EVT_CHANNEL_PRESSURE_LEN = 2
MIDI_EVT_PITCH_BEND_LEN = 3

@dataclass
class MidiHeader:
    format: int
    ntracks: int
    tickdiv: int
    timecode: bool # True for timecode, false for metrical

@dataclass
class MidiTrackMetadata:
    idx: int
    length: int

    def end(self) -> int:
        return self.idx+self.length-1

notes = []
for i in range(128):
    notes.append([0,0].copy())

def skip(file_contents: bytes, idx: int) -> int:
    return 0

def skip_meta(file_contents: bytes, idx: int) -> int:
    len = file_contents[idx+2] + 3
    print("META", [hex(b) for b in file_contents[idx:idx+len]])
    return len

def skip_sysex(file_contents: bytes, idx: int) -> int:
    len = file_contents[idx+1] + 2
    print(len)
    print("SYSEX", [hex(b) for b in file_contents[idx:idx+len]])
    return len + 1

def skip_delta_time(file_contents: bytes, idx: int):
    start_idx = idx
    dt = file_contents[idx]
    while (file_contents[idx] & (1 << 7)):
        idx += 1
        dt <<= 8
        dt |= file_contents[idx]

    print("Delta Time", hex(start_idx),  hex(dt))
    return idx+1

def note_on(file_contents: bytes, idx: int) -> int:
    channel = file_contents[idx] & 0xF
    note = file_contents[idx+1]
    velocity = file_contents[idx+2]
    
    print(f"Note on: {hex(channel)}, {hex(note)}, {hex(velocity)}")
    if note < len(notes):
        notes[note][0] += 1
    return 0

def note_off(file_contents: bytes, idx: int) -> int:
    channel = file_contents[idx] & 0xF
    note = file_contents[idx+1]
    velocity = file_contents[idx+2]

    print(f"Note off: {hex(channel)}, {hex(note)}, {hex(velocity)}")
    if note < len(notes):
        notes[note][1] -= 1
    return 0

@dataclass
class Event:
    evt: int
    len: int
    callback: Callable[[int],int]

MIDI_EVENTS = [
    Event(MIDI_EVT_NOTE_ON, MIDI_EVT_NOTE_ON_LEN, note_on),
    Event(MIDI_EVT_NOTE_OFF, MIDI_EVT_NOTE_OFF_LEN, note_off),
    Event(MIDI_EVT_POLY_PRESS, MIDI_EVT_POLY_PRESS_LEN, skip),
    Event(MIDI_EVT_CONTROLLER, MIDI_EVT_CONTROLLER_LEN, skip),
    Event(MIDI_EVT_PROG_CHNG, MIDI_EVT_PROG_CHNG_LEN, skip),
    Event(MIDI_EVT_CHANNEL_PRESSURE, MIDI_EVT_PITCH_BEND_LEN, skip),
    Event(MIDI_EVT_PITCH_BEND_LEN, MIDI_EVT_PITCH_BEND_LEN, skip),
]

def get_chunk_data_idx(file_contents: bytes, chunk_header: bytes) -> tuple[int,int]:
    data_start = None

    for idx in range(len(file_contents[::4])):
        if file_contents[idx:idx+len(chunk_header)] == chunk_header:
            data_start = idx+len(chunk_header)
            break

    if data_start is None:
        raise RuntimeError("Could not find chunk signature!")
        
    chunk_len = int.from_bytes(file_contents[data_start:data_start+4])

    return data_start+4, chunk_len

def get_midi_track_indices(file_contents: bytes) -> list[MidiTrackMetadata]:
    ret = [MidiTrackMetadata(0,1)]
    while 1:
        try:
            track_start, track_length = get_chunk_data_idx(file_contents[ret[-1].end():], MIDI_TRACK_START)
            ret.append(MidiTrackMetadata(ret[-1].end()+track_start, track_length))
        except:
            break

    return ret[1:]

def step_track(file_contents: bytes, idx) -> int:

    event_byte = file_contents[idx]

    match event_byte:

        case 0xF0:
            new_idx = idx+skip_sysex(file_contents, idx)
            return skip_delta_time(file_contents, new_idx)

        case 0xFF:
            meta_len = skip_meta(file_contents, idx)
            ret = idx + meta_len
            # End of track event has 3 len
            if meta_len > 3:
                ret = skip_delta_time(file_contents, ret)
            return ret

    # MIDI event
    for midi_event_idx in range(len(MIDI_EVENTS)):
        if (event_byte >> 4) ==  MIDI_EVENTS[midi_event_idx].evt:
            extra_bytes = MIDI_EVENTS[midi_event_idx].callback(file_contents, idx)
            new_idx =  idx+MIDI_EVENTS[midi_event_idx].len+extra_bytes
            print("MIDI event", hex(idx), hex(event_byte))
            return skip_delta_time(file_contents, new_idx)

    print(f"skip: {hex(idx)}, {hex(file_contents[idx])}")
    return idx + 1


def parse_midi_header(file_contents: bytes) -> MidiHeader:
    # Find header
    data_idx, _ = get_chunk_data_idx(file_contents, MIDI_HEADER_START)

    format = int.from_bytes(file_contents[data_idx:data_idx+2])
    data_idx+=2

    ntracks = int.from_bytes(file_contents[data_idx:data_idx+2])
    data_idx+=2

    tickdiv = int.from_bytes(file_contents[data_idx:data_idx+2])
    data_idx+=2
    tickdiv &= ~(1<<15)
    
    timecode = bool(tickdiv & (1 << 15))    

    return MidiHeader(format=format, ntracks=ntracks, tickdiv=tickdiv, timecode=timecode)

if __name__ == "__main__":


    MIDI_FILE_PATH = r"C:\Users\Alex\Downloads\pwrply.mid"

    with open(MIDI_FILE_PATH, "rb") as f:
        file_contents = f.read()

        header = parse_midi_header(file_contents)
        print(header.__dict__)

        track_locations = get_midi_track_indices(file_contents)
        print([t.__dict__ for t in track_locations])
    
        assert(header.ntracks == len(track_locations))

        if header.format == 0:
            cur_idx = track_locations[0].idx+1
            while cur_idx < track_locations[0].end():
                cur_idx = step_track(file_contents, cur_idx)
    
        if header.format == 1:
            track_indices = []
            track_locations = track_locations[1:]
            for track in track_locations:
                track_indices.append(skip_delta_time(file_contents, track.idx))

            finished = False

            while not finished:
                finished = True
                for track_num in range(len(track_locations)):
                    if track_indices[track_num] < track_locations[track_num].end():
                        track_indices[track_num] = step_track(file_contents, track_indices[track_num])
                        finished = finished and False
                    else:
                        finished = finished and True

        for idx, counts in enumerate(notes):
            if counts != [0,0]:
                print(idx, counts, sum(counts))