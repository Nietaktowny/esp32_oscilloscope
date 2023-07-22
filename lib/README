# Libraries overview

## Use of lib folder

The lib folder contains all private components of current project.

I will try to describe each component in it's own docs folder. There i will only provide list of used components, and main usage of each.

  

## List of components

Currently this is overview of used components:

- **CMock** - Althrough CMock is part of Espressif platform this was added here, so CMock was also available in native platform. It's only library that doesn't keep the conventions of PlatformIO libs folders usage.
It doesn't have unit tests implemented by me.

- **lcd_controller** - It's main usage is all interactions LCD screen built-in on ESP Wrover Kit.
The unit tests are stored in *test\desktop\test_lcd_controller* folder for native platform, and *test\embedded\test_lcd_controller* folder for espressif platform (tests running on actual device).

## CMock
Each component have it's own CMock files (headers in include folder, and source files in src folder) for mocking. These are generated from command line with `ruby cmock.rb` API and updated manually so it's not advised to make any changes to them.

## List of TODOs

 - [x] Prepare example structure of lib dep (example lib was lcd_controller)
 - [x] Prepare unit tests and mocking for components on one example component
 - [x] Prepare basic README as a place to use for notes about libraries
 - [ ] Implement lcd_controller component
 - [ ] Libraries platform currently needs to be specified as '*' in lib deps, to allow testing on native platform - is there better way?
