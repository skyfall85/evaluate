import numpy as np

tstep_vec=[3200]

for tstep in tstep_vec:
	in_fname='of_'+str(tstep)+'000.npy'
	data=np.load(in_fname)
	xsize=int(np.sqrt(len(data)))
	ysize=xsize
	data=data.reshape((xsize,ysize))
	out_fname='orientation_'+str(tstep)+'.txt'
	np.savetxt(out_fname,data)
	












