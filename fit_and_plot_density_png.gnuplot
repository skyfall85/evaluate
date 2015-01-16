f(x)=1.0/(x*b*sqrt(2.0*pi))*exp(-(log(x)-a)*(log(x)-a)/(2.0*b*b));
tsteps="50 100 200 400 600 800 1000 1250 1500 2000 2500 3000"
thv="95"
do for [th in thv]{
do for [i in tsteps]{
file=sprintf("radius_density_data%s_%s.txt",i,th)
fit f(x) file via a,b
p f(x), file 
max_y = GPVAL_DATA_Y_MAX
set xlabel "R/<R>" font ",22" 
set ylabel "Frequency" font ",22" offset 1,0
set xtics font ",14"
set ytics 0.2 font ",14"
tit=sprintf("Grain radius distribution \n after %s.000 timestep",i)
set title tit offset 0,1  font ",25"
set style fill solid 1.00 border 0
p [0.0:3.5][:1.1] file w boxes linecolor rgb "#3366FF" t "" , f(x) lw 5 lc rgb "red" t "" 
out_fname=sprintf("maggrad_rad_dist_%s_%st_phys_nucl.png",i,th)
set terminal pngcairo size 768,512
set out out_fname
rep
set out
set term x11
}
}
