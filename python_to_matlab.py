from scipy import io as sio
import numpy as np

tstep_vec=[3200]

for tstep in tstep_vec:
	in_fname='pf_'+str(tstep)+'000.npy'
	data=np.load(in_fname)
	xsize=int(np.sqrt(len(data)))
	ysize=xsize
	data=data.reshape((xsize,ysize))
	out_fname='data_'+str(tstep)+'t.mat'
	sio.savemat(out_fname,dict(data=data),oned_as='row')

	in_fname='of_'+str(tstep)+'000.npy'
	of_data=np.load(in_fname)
	xsize=int(np.sqrt(len(of_data)))
	ysize=xsize
	of_data=of_data.reshape((xsize,ysize))
	out_fname='ori_data_'+str(tstep)+'t.mat'
	sio.savemat(out_fname,dict(of_data=of_data),oned_as='row')
	












