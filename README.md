# JPEG Recovery

A forensic data recovery program that extracts JPEG images from raw disk images or memory card dumps.

## Description

This tool scans through a binary file (typically a forensic image of a storage device) and recovers JPEG files by identifying JPEG file signatures. It's particularly useful for recovering deleted photos from memory cards or disk images.

## Features

- Automatic JPEG signature detection
- Sequential file naming (000.jpg, 001.jpg, etc.)
- Handles multiple consecutive JPEG files
- Works with raw disk images and memory dumps
- 512-byte block-aligned reading (FAT filesystem compatible)

## How It Works

The program:
1. Reads the input file in 512-byte blocks
2. Searches for JPEG file signatures (`0xFF 0xD8 0xFF 0xE0` through `0xFF 0xD8 0xFF 0xEF`)
3. When a signature is found, creates a new output file
4. Continues writing blocks to the current output file until a new JPEG is found
5. Names recovered files sequentially: `000.jpg`, `001.jpg`, `002.jpg`, etc.

## Requirements

- C compiler (GCC/Clang)
- Standard C library

## Compilation

```bash
gcc -o recover recover.c
```

Or with debugging symbols:

```bash
gcc -ggdb3 -O0 -Wall -Werror -o recover recover.c
```

## Usage

```bash
./recover [input_file]
```

### Example:

```bash
# Recover JPEGs from a memory card image
./recover card.raw

# Recover JPEGs from a disk image
./recover forensic_image.dd
```

## Output

The program creates JPEG files in the current directory:
- `000.jpg` - first recovered image
- `001.jpg` - second recovered image
- `002.jpg` - third recovered image
- ... and so on

## JPEG Signature Detection

The program identifies JPEG files by their characteristic header bytes:

```
Byte 0: 0xFF
Byte 1: 0xD8
Byte 2: 0xFF
Byte 3: 0xE0 - 0xEF (last 4 bits can vary)
```

This signature appears at the beginning of every JPEG file and is used to determine where each image starts.

## Use Cases

- **Digital Forensics** - recovering images from seized devices
- **Data Recovery** - restoring accidentally deleted photos
- **Memory Card Recovery** - extracting photos from corrupted SD cards
- **Educational** - learning about file signatures and data structures

## Technical Details

- Reads data in 512-byte blocks (standard FAT block size)
- Uses bitwise AND operation `(buffer[3] & 0xf0) == 0xe0` to check JPEG APP markers
- Maintains state to track whether currently writing a JPEG
- Closes previous file before opening new one when signature detected

## Limitations

- Only recovers JPEG files (no other image formats)
- Requires input file to be block-aligned (512 bytes)
- Cannot recover fragmented files
- Assumes JPEGs are stored contiguously
- Does not verify file integrity (recovered files may be corrupted if source was damaged)

## Error Handling

The program handles:
- Missing command-line arguments
- File opening failures
- Output file creation failures

### Error Messages:

- `"Incorrect input"` - wrong number of arguments
- `"error"` - cannot create output file

### Exit Codes:

- `0` - successful recovery
- `1` - error occurred

## Example Scenario

```bash
# You have a memory card image file
$ ls -lh
-rw-r--r-- 1 user user 8.0M card.raw

# Run the recovery tool
$ ./recover card.raw

# Check recovered files
$ ls -lh *.jpg
-rw-r--r-- 1 user user 245K 000.jpg
-rw-r--r-- 1 user user 183K 001.jpg
-rw-r--r-- 1 user user 421K 002.jpg
-rw-r--r-- 1 user user 312K 003.jpg
```

## File Structure

```
.
├── recover.c     # Main program source
└── README.md     # Documentation
```

## Important Notes

- **Overwrites existing files**: If `000.jpg`, `001.jpg`, etc. already exist, they will be overwritten
- **Works best with**: Fresh memory card images, unfragmented filesystems
- **May not work with**: Heavily fragmented drives, encrypted storage, compressed filesystems
