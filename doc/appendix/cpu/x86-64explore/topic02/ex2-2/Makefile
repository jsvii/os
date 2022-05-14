MAKE = make -r
NASM = nasm
BOCHS = bochs -q -f
BOCHSRC = bochsrc.bxrc
MERGE = ../../z_tools/merge.exe
COPY = cp
DEL = rm -rf

default:
	$(MAKE) img

pdemo.img:demo.asm
	$(NASM) demo.asm -o pdemo.img

boot: boot.asm
	$(NASM) boot.asm -o boot

setup: setup.asm
	$(NASM) setup.asm -o setup
	
demo.img: pdemo.img boot setup 
	$(MERGE)
	$(COPY) pdemo.img demo.img

img:
	$(MAKE) demo.img

run:demo.img bochsrc.bxrc
	$(BOCHS) bochsrc.bxrc
	
clean :
	$(DEL) setup
	$(DEL) boot
	$(DEL) parport.out
	$(DEL) pdemo.img 
	$(DEL) demo.img 

