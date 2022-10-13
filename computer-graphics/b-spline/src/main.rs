use std::cell::RefCell;
use std::rc::Rc;

use glib::clone;
use gtk::gdk::BUTTON_PRIMARY;
use gtk::gdk::BUTTON_SECONDARY;
use gtk::glib;
use gtk::prelude::*;
use gtk::GestureClick;
use gtk::{Application, ApplicationWindow, DrawingArea};

const APP_ID: &str = "BSpline";

fn main() {
    let app = Application::builder().application_id(APP_ID).build();

    app.connect_activate(build_ui);

    app.run();
}

fn build_ui(app: &Application) {
    let lines = Rc::new(RefCell::new(vec![Vec::<[f64; 2]>::new()]));
    let area = DrawingArea::builder()
        .content_height(1024)
        .content_width(1024)
        .build();
    area.set_draw_func(clone!(@strong lines => move |_, context, _, _| {
        for line in lines.borrow().iter() {
            if line.is_empty() {
                continue;
            }
            context.move_to(line[0][0], line[0][1]);
            for point in line.into_iter().skip(1) {
                context.line_to(point[0], point[1]);
            }
        }
        context.stroke().expect("Can't stroke!");
    }));

    let gesture_click_primary = GestureClick::builder().button(BUTTON_PRIMARY).build();
    gesture_click_primary.connect_pressed(clone!(@weak area, @weak lines => move |_, _, x, y| {
        if let Some(line) = lines.borrow_mut().last_mut() {
                line.push([x, y]);
        } else {
            lines.borrow_mut().push(Vec::<[f64; 2]>::new());
        }
        area.queue_draw();
    }));
    area.add_controller(&gesture_click_primary);

    let gesture_click_secondary = GestureClick::builder().button(BUTTON_SECONDARY).build();
    gesture_click_secondary.connect_pressed(clone!(@weak lines => move |_, _, _, _| {
        lines.borrow_mut().push(Vec::<[f64; 2]>::new());
    }));
    area.add_controller(&gesture_click_secondary);

    let window = ApplicationWindow::builder()
        .application(app)
        .title("B Spline")
        .child(&area)
        .build();

    window.present();
}
