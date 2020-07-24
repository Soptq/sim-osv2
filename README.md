# sim-osv2

A toy bootloader + kernel that implemented the follow:
1. GDT
2. Bootloader to boot our kernel
3. IDT 
4. IRQ
5. IRQ Timer
6. Keybord driver
7. Mouse driver
8. VGA driver
9. Memory allocator
10. Paging
11. Virtual memory
... More is coming

### Error Code

|Error Code | Definition |
|---|---|
|0x0001|NO FREE FRAMES!|
|0x0002|Heap initialization failed! Start address is not aligned|
|0x0003|Heap initialization failed! End address is not aligned|
|0x0004|Heap expand failed! The requested new_size is smaller than the exists|
|0x0005|Heap expand failed! The requested new_size is out of max_address|
|0x0006|Heap contract failed! The requested new_size is larger than the exists|
|0x0007|Heap free() error! Header's magic number don't match! Memory is disrupted!|
|0x0008|Heap free() error! Footer's magic number don't match! Memory is disrupted!|
|0x0009|Heap free() error! Iterator our of the index bound!|
|0x000a|Ordered array insertion failed! Function less_than not configured!|
|0x000b|Ordered array lookup failed! lookup index is out of the bound!|