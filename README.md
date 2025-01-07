# CBFTP TimeJobSection

A CBFTP fork (revision 1274) that enhances time management for jobs and sections with an improved user interface for time selection and intelligent timeout handling.

## Added Features

### Smart Timeout Management
- Enhanced timeout handling for spread jobs with active transfers:
  - Detects active FXP transfers before applying timeouts
  - Automatically extends timeout by 50% when transfers are in progress
  - Prevents data loss from premature job termination
  - Logs timeout extensions for monitoring
  - Only terminates jobs when:
    - No active transfers are detected, or
    - Extended timeout period has also expired

### Section Time Management
- Added predefined time options for the "Max Time (seconds)" field in section settings
- Time options include:
  - 15 seconds
  - 30 seconds
  - 1 minute
  - 1 minute 30 seconds
  - 2 minutes
  - 3 minutes
  - 5 minutes
  - 10 minutes
  - 15 minutes
  - 30 minutes
  - 1 hour
  - 2 hours
  - 4 hours
  - 8 hours
  - 12 hours
  - 24 hours
  - Unlimited (0)
  - Use global (-1)

### User Interface Improvements
- Added arrow key navigation for time selection fields
- Consistent time selection interface across site and section settings
- Improved user experience with direct arrow key handling without requiring explicit field activation

### Implementation Details
The smart timeout system implements:
1. Active transfer detection
   - Monitors FXP transfer status
   - Tracks ongoing file operations
   - Prevents data loss from premature termination

2. Dynamic timeout adjustment
   - Base timeout from section settings
   - Extension logic for active transfers
   - Configurable extension parameters
   - Status tracking and reporting

3. Graceful termination
   - Completion of active transfers
   - Clean state management
   - Detailed logging of timeout extensions

## Changes from Original CBFTP

1. Modified `src/ui/screens/editsectionscreen.cpp`:
   - Changed time input from numeric field to dropdown selection
   - Added predefined time options
   - Implemented arrow key navigation
   - Unified time selection behavior with site settings

2. Added time selection helper functions similar to site settings

## Installation

Follow the original CBFTP installation instructions. This fork maintains compatibility with the original build process.

## Usage

The new time selection interface can be accessed in:
- Section settings (Edit Section screen)
- The time values can be changed using arrow keys (left/right)
- Values are displayed in human-readable format (e.g., "1h" for 3600 seconds)

## Contributing

Feel free to submit issues and pull requests to improve the time management features or add new enhancements.

## License

This fork maintains the original CBFTP license.

## Credits

- Original CBFTP developers
- Fork maintainer: [ZarTek-Creole](https://github.com/zarTek-Creole/)
- Contributors to the time management enhancements

## Original CBFTP Repository

The original CBFTP can be found at: https://github.com/cbftp/cbftp 

## GitHub Actions Workflows and CI/CD Pipeline

This project uses GitHub Actions for continuous integration and deployment. The following workflows are included:

### Build Workflow
The build workflow is responsible for compiling the project. It is triggered on every push and pull request to the `main` branch.

### Test Workflow
The test workflow runs the project's test suite. It is triggered on every push and pull request to the `main` branch.

### Deploy Workflow
The deploy workflow handles the deployment of the project. It is triggered on every push to the `main` branch and on the creation of new tags.

### Using the Workflows
To use the GitHub Actions workflows, simply push your changes to the repository. The workflows will be triggered automatically based on the specified events.

For more details on the workflows, refer to the `.github/workflows` directory in the repository.
