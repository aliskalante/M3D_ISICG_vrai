#include "lab_work_manager.hpp"
#include "imgui.h"
#include "lab_work_1/lab_work_1.hpp"
#include "lab_work_2/lab_work_2.hpp"
#include "lab_work_3/lab_work_3.hpp"
#include "lab_work_4/lab_work_4.hpp"
#include "lab_work_5/lab_work_5.hpp"
#include "lab_work_6/lab_work_6.hpp"

namespace M3D_ISICG
{
	LabWorkManager::LabWorkManager()
	{
		_current = new LabWork1();
		_type	 = TYPE::LAB_WORK_1;
	}

	void LabWorkManager::drawMenu()
	{
		if ( ImGui ::MenuItem( "tp 1" ) )
		{
			if ( _type != TYPE ::LAB_WORK_1 ) 
			{
				// Keep window size .
				const int w = _current->getWindowWidth();
				const int h = _current->getWindowHeight();
				delete _current;			  
				_current = new LabWork1();	  
				_type	 = TYPE ::LAB_WORK_1; 
				_current->resize( w, h );	  
				_current->init();			  
			}
		}
		
		if ( ImGui ::MenuItem( "tp 2" ) )
		{
			if ( _type != TYPE ::LAB_WORK_2 ) 
			{
				// Keep window size .
				const int w = _current->getWindowWidth();
				const int h = _current->getWindowHeight();
				delete _current;			  
				_current = new LabWork2();	  
				_type	 = TYPE ::LAB_WORK_2; 
				_current->resize( w, h );	  
				_current->init();			  
			}
		}

		if ( ImGui ::MenuItem( "tp 3" ) )
		{
			if ( _type != TYPE ::LAB_WORK_3 ) 
			{
				
				const int w = _current->getWindowWidth();
				const int h = _current->getWindowHeight();
				delete _current;			  
				_current = new LabWork3();	  
				_type	 = TYPE ::LAB_WORK_3; 
				_current->resize( w, h );	  
				_current->init();			  
			}
		}

		if ( ImGui ::MenuItem( "tp 4" ) )
		{
			if ( _type != TYPE ::LAB_WORK_4 ) 
			{
				
				const int w = _current->getWindowWidth();
				const int h = _current->getWindowHeight();
				delete _current;			  
				_current = new LabWork4();	  
				_type	 = TYPE ::LAB_WORK_4; 
				_current->resize( w, h );	  
				_current->init();			  
			}
		}

		if ( ImGui ::MenuItem( "tp 5" ) )
		{
			if ( _type != TYPE ::LAB_WORK_5 ) 
			{
				
				const int w = _current->getWindowWidth();
				const int h = _current->getWindowHeight();
				delete _current;			  
				_current = new LabWork5();	  
				_type	 = TYPE ::LAB_WORK_5; 
				_current->resize( w, h );	  
				_current->init();			  
			}
		}

		if ( ImGui ::MenuItem( "tp 6" ) )
		{
			if ( _type != TYPE ::LAB_WORK_6 ) 
			{
				
				const int w = _current->getWindowWidth();
				const int h = _current->getWindowHeight();
				delete _current;			  
				_current = new LabWork6();	  
				_type	 = TYPE ::LAB_WORK_6; 
				_current->resize( w, h );	  
				_current->init();			  
			}
		}
	}
} // namespace M3D_ISICG
