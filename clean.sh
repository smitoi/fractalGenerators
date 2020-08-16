folders=(chaosGameFractal kochSnowflake mandelbrotSet)
for folder in ${folders[@]}; do
	cd $folder
	make clean
	cd ..
done

