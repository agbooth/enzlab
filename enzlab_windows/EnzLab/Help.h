/*
	context numbers for help in EnzLab
*/

#define HELPFILE		"enzlab.hlp"
/* 
	indices
		start at 0x0000
	cnt_xxx
	file : CONTENTS
*/
#define cnt_contents		0x0000
#define cnt_highlight		0x0001
#define cnt_howto		0x0002
#define cnt_dlg			0x0003
#define cnt_menus		0x0004

/*
	howtos
		start at 0x0101
	ht_xxx
	file : HOWTO
*/
#define ht_windows		0x0101
#define ht_menu			0x0102
#define ht_plan			0x0103
#define ht_choose_params	0x0104
#define ht_choose_vol		0x0105
#define ht_choose_ph		0x0106
#define ht_choose_s		0x0107
#define ht_choose_i		0x0108
#define ht_expt_run		0x0109
#define ht_save			0x010A
#define ht_load			0x010B
#define ht_precision		0x010C
#define ht_kinetics		0x010D
#define ht_phprofile		0x010E
#define ht_restart		0x010F
#define ht_end			0x0110

/*
	dialog helps
		start at 0x0201
	dlg_xxx
	file : DLG
*/
#define dlg_anlist		0x0201
#define dlg_enzsel		0x0202
#define dlg_intro		0x0203
#define dlg_kinexam		0x0204
#define dlg_kinres		0x0205
#define dlg_kinsel		0x0206
#define dlg_load		0x0207
#define dlg_newday		0x0208
#define dlg_phpres		0x0209
#define dlg_phpsel		0x020A
#define dlg_precres		0x020B
#define dlg_precsel		0x020C
#define dlg_reslis		0x020D
#define dlg_save		0x020E

/*
	background
		start at 0x0401
	bg_xxx
	file : BGRND
*/
#define bg_introduction		0x0301
#define bg_background		0x0302
#define bg_program		0x0303
#define bg_abbrev		0x0304
#define bg_units		0x0305
#define bg_record		0x0306

/*
	what next
		start at 0x0501
	wn_xxx
	file : WHATNEXT
*/
#define wn_sel_enz		0x0401
#define wn_do_expts		0x0402
#define wn_kinetics		0x0403
#define wn_precision	0x0404
#define wn_phprofile	0x0405
#define wn_inhibitor	0x0406
