all: asm1.bin

asm1.bin: asm1.asm
	nasm -o $@ -f bin $<

clean:
	rm -v *.bin
