use std::cell::RefCell;
use std::rc::Rc;

use cubic_b_splines::Curve;
use glib::clone;
use gtk::gdk::{BUTTON_MIDDLE, BUTTON_PRIMARY, BUTTON_SECONDARY};
use gtk::glib;
use gtk::prelude::*;
use gtk::{Application, ApplicationWindow, DrawingArea};
use gtk::{GestureClick, GestureDrag};

const APP_ID: &str = "BSpline";

mod cubic_b_splines;

fn main() {
    let app = Application::builder().application_id(APP_ID).build();

    app.connect_activate(build_ui);

    app.run();
}

const WIDTH: usize = 1024;
const HEIGHT: usize = 1024;

fn build_ui(app: &Application) {
    let curves = Rc::new(RefCell::new(vec![Curve::<HEIGHT, WIDTH>::new()]));
    let area = DrawingArea::builder()
        .content_height(HEIGHT as i32)
        .content_width(WIDTH as i32)
        .build();
    area.set_draw_func(clone!(@strong curves => move |_, context, _, _| {
        for curve in curves.borrow().iter() {
            if curve.is_empty() {
                continue;
            }
            context.set_source_rgb(0.0, 1.0, 1.0);
            context.move_to(curve[0][0], curve[0][1]);
            for point in curve.iter().skip(1) {
                context.line_to(point[0], point[1]);
            }
            context.stroke().expect("Can't stroke!");
            if let Some(lines) = curve.get_lines() {
                if lines.is_empty() {
                    continue;
                }
                context.set_source_rgb(1.0, 0.0, 0.0);
                context.move_to(lines[0][0], lines[0][1]);
                for point in lines.iter().skip(1) {
                    context.line_to(point[0], point[1]);
                }
                context.stroke().expect("Can't stroke!");
            }
        }
    }));

    let gesture_click_primary = GestureClick::builder().button(BUTTON_PRIMARY).build();
    gesture_click_primary.connect_pressed(clone!(@weak area, @weak curves => move |_, _, x, y| {
        if let Some(curve) = curves.borrow_mut().last_mut() {
            curve.push([x, y])
        } else {
            curves.borrow_mut().push(Curve::new());
        }
        area.queue_draw();
    }));
    area.add_controller(&gesture_click_primary);

    let gesture_click_middle = GestureClick::builder().button(BUTTON_MIDDLE).build();
    gesture_click_middle.connect_pressed(clone!(@weak area, @weak curves => move |_, _, _, _| {
        if let Some(curve) = curves.borrow_mut().last_mut() {
            curve.line();
        }
        curves.borrow_mut().push(Curve::new());
        area.queue_draw();
    }));
    area.add_controller(&gesture_click_middle);

    let gesture_drag_secondary = GestureDrag::builder().button(BUTTON_SECONDARY).build();
    gesture_drag_secondary.connect_drag_end(
        clone!(@weak area, @weak curves => move |drag, delta_x, delta_y| {
            if let Some(origin) = drag.start_point() {
                let origin = [origin.0, origin.1];
                for curve in curves.borrow_mut().iter_mut() {
                    if curve.is_empty() {
                        continue;
                    }
                    if let Some(point) = curve.catch(&origin) {
                        point[0] += delta_x;
                        point[1] += delta_y;
                        if curve.get_lines().is_some() {
                            curve.line();
                        }
                        area.queue_draw();
                        break;
                    }
                }
            }
        }),
    );
    area.add_controller(&gesture_drag_secondary);

    let window = ApplicationWindow::builder()
        .application(app)
        .title("B Spline")
        .child(&area)
        .build();

    window.present();
}
