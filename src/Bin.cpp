#include <SFGUI/Bin.hpp>
#include <iostream>

namespace sfg {

Bin::Bin() :
	Container()
{
	OnAdd.Connect( &Bin::HandleAdd, this );
}

Widget::Ptr Bin::GetChild() const {
	if( GetChildren().size() < 1 ) {
		return Widget::Ptr();
	}

	return *GetChildren().begin();
}

void Bin::HandleAdd( Widget::Ptr /*widget*/, Widget::Ptr child ) {
	if( GetChildren().size() > 1 ) {
		std::cerr << "SFGUI warning: Only one widget can be added to a Bin." << std::endl;
		Remove( child );
		return;
	}

	// Make sure the Bin is large enough.
	QueueResize( child );
}

void Bin::QueueResize( Widget::Ptr widget ) {
	if( widget == shared_from_this() ) {
		Widget::QueueResize( widget );
		return;
	}

	if( !IsChild( widget ) ) {
		return;
	}

	// Set requisition of Bin to child's requisition.
	RequestSize( widget->GetRequisition() );
}

}
