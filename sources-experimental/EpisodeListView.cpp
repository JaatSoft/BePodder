#include "EpisodeListView.h"
#include "SubscriptionColumn.h"

#include "MyColumnTypes.h"
#include <Box.h>
#include <Catalog.h>
#include <ControlLook.h>
#include <ScrollBar.h>
#include <StringFormat.h>
#include <Window.h>

#include "EpisodeListItem.h"
#include "FileStatusColumn.h"
#include "Colors.h"
#include "StatusView.h"

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "DL-Episode-ListView"

EpisodeListView::EpisodeListView(BRect r):
BColumnListView(r,"EpisodeListView",B_FOLLOW_ALL, B_WILL_DRAW|B_NAVIGABLE,B_NO_BORDER,true)
{
	BColumn *icon = new BBitmapColumn(B_TRANSLATE("Icon"),16,16,16,B_ALIGN_CENTER);
	BColumn *title = new BStringColumn(B_TRANSLATE("Title"),140,10,500,B_TRUNCATE_MIDDLE,B_ALIGN_LEFT);
	BColumn *date  = new BPositiveDateColumn(B_TRANSLATE("Date"),70,10,250,B_ALIGN_LEFT);
	BColumn *size  = new BPositiveSizeColumn(B_TRANSLATE("Size"),80,10,150,B_ALIGN_LEFT);
	
	int index = 0;
	AddColumn(icon,index++);
	AddColumn(title,index++);
	AddColumn(date,index++);
	AddColumn(size,index++);
	AddColumn(new FileStatusColumn(B_TRANSLATE("Status"),200,110,400,0,B_ALIGN_LEFT),index++);
	
	SetColumnFlags((column_flags)(B_ALLOW_COLUMN_REMOVE|B_ALLOW_COLUMN_RESIZE|B_ALLOW_COLUMN_POPUP|B_ALLOW_COLUMN_MOVE));
	SetSelectionMode(B_MULTIPLE_SELECTION_LIST);
	SetSortingEnabled(true);
	SetSortColumn(date,false,false);
	BScrollBar* scrollBar
		= (BScrollBar*)FindView("horizontal_scroll_bar");
	AddStatusView(fStatusView=new StatusView(scrollBar));
}

void				
EpisodeListView::SelectionChanged(){
	uint32 buttons = 0;
	
	BMessage *msg = Window()->CurrentMessage();
	
	if(msg)	//don't remove.
		msg->FindInt32("buttons", (int32 *)&buttons) ;
	
	if(SelectionMessage())		
		SelectionMessage()->ReplaceInt32("buttons",buttons);

//	EpisodeListItem* sel = (EpisodeListItem*)CurrentSelection();
	
//	SelectionMessage()->RemoveName("entry_ref");
//	SelectionMessage()->AddRef("entry_ref",&sel->fRef);
					
	BColumnListView::SelectionChanged();
}


void				
EpisodeListView::AddRow(BRow* row, BRow *parent){
	BColumnListView::AddRow(row,parent);
	ExpandOrCollapse(row, true);
	UpdateCount();
}

void
EpisodeListView::RemoveRow(BRow* row){
	BColumnListView::RemoveRow(row);
	UpdateCount();
}

void				
EpisodeListView::AddRow(BRow* row, int32 index, BRow *parent){
	BColumnListView::AddRow(row,index,parent);
	ExpandOrCollapse(row, true);
	UpdateCount();
}

void
EpisodeListView::Clear(){
	BColumnListView::Clear();
	UpdateCount();
}

void
EpisodeListView::UpdateCount(){
	BString text;
	static BStringFormat format(B_TRANSLATE("{0, plural,"
		"=0{no episodes}"
		"=1{1 episode}"
		"other{# episodes}}"));
	format.Format(text, CountRows());
	fStatusView->Update(text, "", "");
}

				
//--
