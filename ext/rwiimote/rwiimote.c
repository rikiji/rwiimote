/*  
 * Copyright (C) 2010 Riccardo Cecolin 
 * rkj@playkanji.com - 11 april 2010
 */

#include <ruby.h>
#include <libcwiimote/wiimote.h>
#include <libcwiimote/wiimote_api.h>
#include <libcwiimote/wiimote_event.h>
#include <stdio.h>
#include <math.h>
#define DBGd(x) printf("%s: %d\n",#x,x)

struct wiimote_data_t {
	wiimote_t device;
	char mac[20];
};

#define GetWM(obj, p) Data_Get_Struct(obj, struct wiimote_data_t, p)

static VALUE rb_cWiimote;
static VALUE rb_cLed;
VALUE wiimote_data;

void rwiimote_free_data (struct wiimote_data_t * wmdp) 
{
	free(wmdp);
}
VALUE rwiimote_allocate (VALUE klass)
{
	struct wiimote_data_t * wmdp;
	wiimote_data = Data_Make_Struct(klass, struct wiimote_data_t, 0, rwiimote_free_data, wmdp);
	return wiimote_data;
}
VALUE rb_wiimote_initialize (VALUE self)
{
	int i=0;
	VALUE leds = rb_ary_new();
	VALUE one;
	for (i=0;i<4;i++) {
		one = rb_class_new_instance (0, 0, rb_cLed);
		rb_iv_set(one, "id", INT2FIX(i+1));
		rb_iv_set(one, "wiimote", self);
		rb_ary_push(leds, one);
	}
	rb_iv_set(self,"led",leds);
	return self;
}
VALUE rb_wiimote_connect(VALUE self, VALUE mac)
{
	struct wiimote_data_t * wmdp; 
	GetWM(wiimote_data,wmdp);

	if (TYPE(mac) == T_STRING)
		strcpy(wmdp->mac,StringValuePtr(mac));
	else
		return Qfalse;

	if(wiimote_connect(&(wmdp->device),wmdp->mac)<0)
		return Qfalse;
	else
		return Qtrue;
}
VALUE rb_wiimote_update(VALUE self) 
{
	struct wiimote_data_t * wmdp; 
	GetWM(wiimote_data,wmdp);
	if(wiimote_update(&(wmdp->device))<0)
		return Qfalse;
	else
		return Qtrue;
}
VALUE rb_wiimote_disconnect (VALUE self)
{
	struct wiimote_data_t * wmdp; 
	GetWM(wiimote_data,wmdp);
	wiimote_disconnect(&wmdp->device);
	return Qtrue;
}
VALUE rb_wiimote_flush (VALUE self)
{
	struct wiimote_data_t * wmdp; 
	GetWM(wiimote_data,wmdp);
	while(wiimote_pending(&wmdp->device) > 0)
		wiimote_update(&wmdp->device);
	return Qtrue;
}
VALUE rb_wiimote_open_p (VALUE self)
{
	struct wiimote_data_t * wmdp; 
	GetWM(wiimote_data,wmdp);
	if(wiimote_is_open(&wmdp->device))
		return Qtrue;
	else
		return Qfalse;
}
VALUE rb_wiimote_acc_p (VALUE self) {
	struct wiimote_data_t * wmdp; 
	GetWM(wiimote_data,wmdp);
	return INT2FIX(wmdp->device.mode.acc);
}
VALUE rwiimote_tobool (VALUE val)
{
	VALUE flag;
	switch(TYPE(val)) {
    case T_FIXNUM:
    case T_BIGNUM:
    case T_FLOAT:
		if(NUM2INT(val) == 0)
			flag = Qfalse;
		else
			flag = Qtrue;
		break;
    case T_TRUE:
		flag = Qtrue;
		break;
    case T_FALSE:
    default:
		flag = Qfalse;
	}
	return flag;
}
VALUE rb_wiimote_acc_set (VALUE self, VALUE val) {
	struct wiimote_data_t * wmdp; 
	GetWM(wiimote_data,wmdp);
	VALUE flag = rwiimote_tobool(val);
	if (flag == Qtrue) 
		wmdp->device.mode.acc = 1;
	else
		wmdp->device.mode.acc = 0;
	return flag;
}
VALUE rb_wiimote_left_p (VALUE self)
{
	return rb_funcall(self, rb_intern("key?"), 1, ID2SYM(rb_intern("left")));
}
VALUE rb_wiimote_right_p (VALUE self)
{
	return rb_funcall(self, rb_intern("key?"), 1, ID2SYM(rb_intern("right")));
}
VALUE rb_wiimote_down_p (VALUE self)
{
	return rb_funcall(self, rb_intern("key?"), 1, ID2SYM(rb_intern("down")));
}
VALUE rb_wiimote_up_p (VALUE self)
{
	return rb_funcall(self, rb_intern("key?"), 1, ID2SYM(rb_intern("up")));
}
VALUE rb_wiimote_plus_p (VALUE self)
{
	return rb_funcall(self, rb_intern("key?"), 1, ID2SYM(rb_intern("plus")));
}
VALUE rb_wiimote_two_p (VALUE self)
{
	return rb_funcall(self, rb_intern("key?"), 1, ID2SYM(rb_intern("two")));
}
VALUE rb_wiimote_one_p (VALUE self)
{
	return rb_funcall(self, rb_intern("key?"), 1, ID2SYM(rb_intern("one")));
}
VALUE rb_wiimote_b_p (VALUE self)
{
	return rb_funcall(self, rb_intern("key?"), 1, ID2SYM(rb_intern("b")));
}
VALUE rb_wiimote_a_p (VALUE self)
{
	return rb_funcall(self, rb_intern("key?"), 1, ID2SYM(rb_intern("a")));
}
VALUE rb_wiimote_minus_p (VALUE self)
{
	return rb_funcall(self, rb_intern("key?"), 1, ID2SYM(rb_intern("minus")));
}
VALUE rb_wiimote_home_p (VALUE self)
{
	return rb_funcall(self, rb_intern("key?"), 1, ID2SYM(rb_intern("home")));
}
VALUE rb_wiimote_force_x(VALUE self)
{
	struct wiimote_data_t * wmdp; 
	GetWM(wiimote_data,wmdp);
	return rb_float_new(wmdp->device.force.x);
}
VALUE rb_wiimote_force_y(VALUE self)
{
	struct wiimote_data_t * wmdp; 
	GetWM(wiimote_data,wmdp);
	return rb_float_new(wmdp->device.force.y);
}
VALUE rb_wiimote_force_z(VALUE self)
{
	struct wiimote_data_t * wmdp; 
	GetWM(wiimote_data,wmdp);
	return rb_float_new(wmdp->device.force.z);
}
VALUE rb_wiimote_force_abs(VALUE self)
{
	struct wiimote_data_t * wmdp; 
	GetWM(wiimote_data,wmdp);
	return rb_float_new(sqrt(wmdp->device.force.x*wmdp->device.force.x + wmdp->device.force.y*wmdp->device.force.y + wmdp->device.force.z*wmdp->device.force.z));
}
VALUE rb_wiimote_tilt_x(VALUE self)
{
	struct wiimote_data_t * wmdp; 
	GetWM(wiimote_data,wmdp);
	return rb_float_new(wmdp->device.tilt.x);
}
VALUE rb_wiimote_tilt_y(VALUE self)
{
	struct wiimote_data_t * wmdp; 
	GetWM(wiimote_data,wmdp);
	return rb_float_new(wmdp->device.tilt.y);
}
VALUE rb_wiimote_tilt_z(VALUE self)
{
	struct wiimote_data_t * wmdp; 
	GetWM(wiimote_data,wmdp);
	return rb_float_new(wmdp->device.tilt.z);
}
VALUE rb_wiimote_key(VALUE self, VALUE key)
{
	struct wiimote_data_t * wmdp; 
	GetWM(wiimote_data,wmdp);
	unsigned int mask = 1;

	if(!SYMBOL_P(key))
		return Qfalse;

	unsigned int keyid = rb_to_id(key);

	char * keysymbols[] = {
		"left","right","down","up","plus","two","one","b","a","minus","home",NULL
	};
	unsigned int keyoffset [] = {
		0,1,2,3,4,8,9,10,11,12,15,0
	};

	char i = 0;
	while(keysymbols[i] != NULL) {
		if(keyid == rb_intern(keysymbols[i])){
			mask <<= keyoffset[i];
			break;
		}
		i++;
	}
 
	if(wmdp->device.keys.bits & mask) 
		return Qtrue;
	else
		return Qfalse;

}
VALUE rb_wiimote_led_bits (VALUE self, VALUE n, VALUE val)
{
	struct wiimote_data_t * wmdp; 
	GetWM(wiimote_data,wmdp);

	unsigned int mask = 1;
	VALUE flag = rwiimote_tobool(val);

	if(NUM2INT(n) > 0 && 5 > NUM2INT(n)) {
		mask = mask << (NUM2INT(n)-1);
		if (flag == Qtrue)
			wmdp->device.led.bits |= mask;
		else 
			wmdp->device.led.bits &= ~mask;
	}
	return flag;
}
VALUE rb_wiimote_led (VALUE self)
{
	return rb_iv_get(self,"led");
}
VALUE rb_led_on (VALUE self)
{
	return rb_funcall(rb_iv_get(self,"wiimote"), rb_intern("led_bits"), 2, rb_iv_get(self,"id"), Qtrue);
}
VALUE rb_led_off (VALUE self)
{
	return rb_funcall(rb_iv_get(self,"wiimote"), rb_intern("led_bits"), 2, rb_iv_get(self,"id"), Qfalse);
}
void
Init_rwiimote ()
{
	rb_cWiimote = rb_define_class ("Wiimote", rb_cObject);
	rb_define_alloc_func(rb_cWiimote, rwiimote_allocate);
	rb_define_method (rb_cWiimote, "initialize", rb_wiimote_initialize, 0);

	rb_cLed = rb_define_class("Led",rb_cObject);
	rb_define_method (rb_cLed, "on", rb_led_on, 0);
	rb_define_method (rb_cLed, "off", rb_led_off, 0);

	rb_define_method (rb_cWiimote, "connect", rb_wiimote_connect, 1);
	rb_define_method (rb_cWiimote, "disconnect", rb_wiimote_disconnect, 0);
	rb_define_method (rb_cWiimote, "update", rb_wiimote_update, 0);
	rb_define_method (rb_cWiimote, "open?", rb_wiimote_open_p, 0);
	rb_define_method (rb_cWiimote, "flush", rb_wiimote_flush, 0);

	rb_define_private_method (rb_cWiimote, "led_bits", rb_wiimote_led_bits, 2);
	rb_define_method (rb_cWiimote, "led", rb_wiimote_led, 0);

	rb_define_private_method (rb_cWiimote, "key?", rb_wiimote_key, 1);
	rb_define_method (rb_cWiimote, "a?", rb_wiimote_a_p, 0);
	rb_define_method (rb_cWiimote, "b?", rb_wiimote_b_p, 0);
	rb_define_method (rb_cWiimote, "one?", rb_wiimote_one_p, 0);
	rb_define_method (rb_cWiimote, "two?", rb_wiimote_two_p, 0);
	rb_define_method (rb_cWiimote, "plus?", rb_wiimote_plus_p, 0);
	rb_define_method (rb_cWiimote, "minus?", rb_wiimote_minus_p, 0);
	rb_define_method (rb_cWiimote, "up?", rb_wiimote_up_p, 0);
	rb_define_method (rb_cWiimote, "down?", rb_wiimote_down_p, 0);
	rb_define_method (rb_cWiimote, "left?", rb_wiimote_left_p, 0);
	rb_define_method (rb_cWiimote, "right?", rb_wiimote_right_p, 0);
	rb_define_method (rb_cWiimote, "home?", rb_wiimote_home_p, 0);

	rb_define_method (rb_cWiimote, "acc?", rb_wiimote_acc_p, 0);
	rb_define_method (rb_cWiimote, "acc=", rb_wiimote_acc_set, 1);

	rb_define_method (rb_cWiimote, "force", rb_wiimote_force_abs, 0);
	rb_define_method (rb_cWiimote, "force_x", rb_wiimote_force_x, 0);
	rb_define_method (rb_cWiimote, "force_y", rb_wiimote_force_y, 0);
	rb_define_method (rb_cWiimote, "force_z", rb_wiimote_force_z, 0);

	rb_define_method (rb_cWiimote, "tilt_x", rb_wiimote_tilt_x, 0);
	rb_define_method (rb_cWiimote, "tilt_y", rb_wiimote_tilt_y, 0);
	rb_define_method (rb_cWiimote, "tilt_z", rb_wiimote_tilt_z, 0);
}
