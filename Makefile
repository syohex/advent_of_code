.PHONY: format clean

format:
	git ls-files | grep -E '\.(c|cpp|cc|h)$$' | xargs clang-format -i

clean:
	-find . -name 'a.out' -o 'a.exe' -type f | xargs rm -f
