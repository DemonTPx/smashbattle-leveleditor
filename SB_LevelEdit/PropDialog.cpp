#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#	include "wx/wx.h"
#endif

#include "PropDialog.h"

PropDialog * PropDialog::instance = NULL;

PropDialog::PropDialog(wxWindow* parent, wxWindowID id, const wxPoint& pos) : wxDialog(parent, id, _("Prop"), pos, wxSize(200, 185)){
	instance = this;

	InitializeComponents();
}

PropDialog::~PropDialog() {
	instance = NULL;
}

void PropDialog::InitializeComponents() {
	SetClientSize(200, 165);

	// Static text
	lblSource = new wxStaticText(this, wxID_ANY, _("Source"), wxPoint(5, 5), wxSize(190, 20));

	lblSrcX = new wxStaticText(this, wxID_ANY, _("X:"), wxPoint(5, 30), wxSize(50, 20));
	lblSrcY = new wxStaticText(this, wxID_ANY, _("Y:"), wxPoint(105, 30), wxSize(50, 20));
	lblSrcW = new wxStaticText(this, wxID_ANY, _("Width:"), wxPoint(5, 50), wxSize(50, 20));
	lblSrcH = new wxStaticText(this, wxID_ANY, _("Height:"), wxPoint(105, 50), wxSize(50, 20));
	
	lblDest = new wxStaticText(this, wxID_ANY, _("Destination"), wxPoint(5, 75), wxSize(190, 20));
	
	lblDstX = new wxStaticText(this, wxID_ANY, _("X:"), wxPoint(5, 95), wxSize(50, 20));
	lblDstY = new wxStaticText(this, wxID_ANY, _("Y:"), wxPoint(105, 95), wxSize(50, 20));

	// Input fields
	txtSrcX = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxPoint(60, 30), wxSize(30, 20));
	txtSrcX->SetMaxLength(3);
	txtSrcY = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxPoint(160, 30), wxSize(30, 20));
	txtSrcY->SetMaxLength(3);
	txtSrcW = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxPoint(60, 50), wxSize(30, 20));
	txtSrcW->SetMaxLength(3);
	txtSrcH = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxPoint(160, 50), wxSize(30, 20));
	txtSrcH->SetMaxLength(3);

	txtDstX = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxPoint(60, 95), wxSize(30, 20));
	txtDstX->SetMaxLength(3);
	txtDstY = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxPoint(160, 95), wxSize(30, 20));
	txtDstY->SetMaxLength(3);

	// Buttons
	btnOK = new wxButton(this, wxID_OK, _("&Save"), wxPoint(90, 130), wxSize(50, 30));
	btnOK->SetDefault();
	btnCancel = new wxButton(this, wxID_CANCEL, _("&Cancel"), wxPoint(145, 130), wxSize(50, 30));
}

void PropDialog::GetProp(LEVEL_PROP &prop) {
	long l;
	
	memset(&prop, 0, sizeof(LEVEL_PROP));

	txtSrcX->GetValue().ToLong(&l);
	prop.src.x = (short)l;
	txtSrcY->GetValue().ToLong(&l);
	prop.src.y = (short)l;
	
	txtSrcW->GetValue().ToLong(&l);
	prop.src.w = (short)l;
	txtSrcH->GetValue().ToLong(&l);
	prop.src.h = (short)l;

	txtDstX->GetValue().ToLong(&l);
	prop.dst.x = (short)l;
	txtDstY->GetValue().ToLong(&l);
	prop.dst.y = (short)l;
}

void PropDialog::SetProp(LEVEL_PROP &prop) {
	txtSrcX->SetValue(wxString::Format(_("%d"), prop.src.x));
	txtSrcY->SetValue(wxString::Format(_("%d"), prop.src.y));
	txtSrcW->SetValue(wxString::Format(_("%d"), prop.src.w));
	txtSrcH->SetValue(wxString::Format(_("%d"), prop.src.h));
	
	txtDstX->SetValue(wxString::Format(_("%d"), prop.dst.x));
	txtDstY->SetValue(wxString::Format(_("%d"), prop.dst.y));
}
