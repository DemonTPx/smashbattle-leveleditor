#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#	include "wx/wx.h"
#endif

#include "PropDialog.h"

PropDialog * PropDialog::instance = NULL;

PropDialog::PropDialog(wxWindow* parent, wxWindowID id, const wxPoint& pos) : wxDialog(parent, id, _("Prop"), pos){
	instance = this;

	InitializeComponents();
}

PropDialog::~PropDialog() {
	instance = NULL;
}

void PropDialog::InitializeComponents() {
	wxBoxSizer * sizer = new wxBoxSizer(wxVERTICAL);

	sizer->Add(new wxStaticText(this, wxID_ANY, _("Source")), 0, wxALL, 5);

	wxGridSizer * sourceGridSizer = new wxGridSizer(4);

	sourceGridSizer->Add(new wxStaticText(this, wxID_ANY, _("X:")), 0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
	txtSrcX = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
	txtSrcX->SetMaxLength(3);
	sourceGridSizer->Add(txtSrcX);

	sourceGridSizer->Add(new wxStaticText(this, wxID_ANY, _("Y:")), 0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
	txtSrcY = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
	txtSrcY->SetMaxLength(3);
	sourceGridSizer->Add(txtSrcY);

	sourceGridSizer->Add(new wxStaticText(this, wxID_ANY, _("Width:")), 0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
	txtSrcW = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
	txtSrcW->SetMaxLength(3);
	sourceGridSizer->Add(txtSrcW);

	sourceGridSizer->Add(new wxStaticText(this, wxID_ANY, _("Height:")), 0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
	txtSrcH = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
	txtSrcH->SetMaxLength(3);
	sourceGridSizer->Add(txtSrcH);

	sizer->Add(sourceGridSizer, 0, wxALL, 5);

	sizer->Add(new wxStaticText(this, wxID_ANY, _("Destination")), 0, wxALL, 5);

	wxGridSizer * destGridSizer = new wxGridSizer(4);

	destGridSizer->Add(new wxStaticText(this, wxID_ANY, _("X:")), 0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
	txtDstX = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
	txtDstX->SetMaxLength(3);
	destGridSizer->Add(txtDstX);

	destGridSizer->Add(new wxStaticText(this, wxID_ANY, _("Y:")), 0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
	txtDstY = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
	txtDstY->SetMaxLength(3);
	destGridSizer->Add(txtDstY);

	sizer->Add(destGridSizer, 0, wxALL, 5);

	sizer->Add(CreateStdDialogButtonSizer(wxOK | wxCANCEL), 0, wxALIGN_RIGHT | wxALL, 5);

	SetSizerAndFit(sizer);
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
