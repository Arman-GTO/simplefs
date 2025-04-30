# Simple File System

This project implements a simple file system in C. It provides basic file operations such as creating, reading, writing, deleting files, and listing all files stored on a virtual disk.

## Features

- **File Operations**:
  - Create a file
  - Write data to a file
  - Read data from a file
  - Delete a file
  - List all files on the disk
- **Virtual Disk**:
  - The file system uses a virtual disk (`disk.img`) to store file data and metadata.
  - The disk size is configurable and defaults to 1MB.
- **Metadata Management**:
  - Metadata includes file names, sizes, and block allocation information.
  - A bitmap is used to track free and used blocks on the disk.

## Project Structure

```
├── fs.c       # Implementation of the file system
├── fs.h       # Header file defining the file system API
├── main.c     # Command-line interface for interacting with the file system
├── Makefile   # Build script
```

## Build Instructions

To build the project, use the provided `Makefile`:

```sh
make
```

This will generate an executable named `fs`.

## Usage

Run the `fs` executable with the following commands:

```sh
./fs help
```

Displays usage instructions.

### Commands

1. **Create a File**:

   ```sh
   ./fs create <filename>
   ```

   Example:

   ```sh
   ./fs create myfile.txt
   ```

2. **Write to a File**:

   ```sh
   ./fs write <filename> <data>
   ```

   Example:

   ```sh
   ./fs write myfile.txt "Hello, World!"
   ```

3. **Read a File**:

   ```sh
   ./fs read <filename>
   ```

   Example:

   ```sh
   ./fs read myfile.txt
   ```

4. **Delete a File**:

   ```sh
   ./fs delete <filename>
   ```

   Example:

   ```sh
   ./fs delete myfile.txt
   ```

5. **List All Files**:
   ```sh
   ./fs list
   ```

## File System Details

- **Disk File**: The virtual disk is stored in a file named `disk.img`.
- **Disk Size**: 1MB by default.
- **Block Size**: 512 bytes.
- **Maximum Files**: 128 files.
- **Maximum File Size**: 4KB.
- **Metadata**: The first block of the disk is reserved for metadata.

## Example

```sh
# Create a file
./fs create example.txt

# Write data to the file
./fs write example.txt "This is a test."

# Read the file
./fs read example.txt

# List all files
./fs list

# Delete the file
./fs delete example.txt
```

## Limitations

- Maximum file size is limited to 4KB.
- The total number of files is limited to 128.
- The disk size is fixed at 1MB.

## License

This project is for educational purposes and does not include a specific license.
