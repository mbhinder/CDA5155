/*
 * pipeline.hh
 * 03/26/2010
 * Mohanpreet Bhinder
 */

#ifndef PIPELINE_HH
#define PIPELINE_HH

class Pipeline
{
	public:
		//The five stages of the Tomasulo algo
		void pipeline_fetch();
		void pipeline_issue();
		void pipeline_execute();
		void pipeline_write();	
		void pipeline_commit();
		Pipeline();
};
#endif
