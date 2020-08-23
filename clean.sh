folders=(chaosGameFractal kochSnowflake complex)
for folder in ${folders[@]}; do
	cd $folder
	make clean
	cd ..
done

