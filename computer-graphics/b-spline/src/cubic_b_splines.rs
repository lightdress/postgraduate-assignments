fn get_basis(i: usize) -> Box<dyn Fn(f64) -> f64> {
    let i = i as f64;
    Box::new(move |t| {
        if i <= t && t < i + 1.0 {
            let u = t - i;
            u.powi(3) / 6.0
        } else if i + 1.0 <= t && t < i + 2.0 {
            let u = t - (i + 1.0);
            (-3.0 * u.powi(3) + 3.0 * u.powi(2) + 3.0 * u + 1.0) / 6.0
        } else if i + 2.0 <= t && t < i + 3.0 {
            let u = t - (i + 2.0);
            (3.0 * u.powi(3) - 6.0 * u.powi(2) + 4.0) / 6.0
        } else if i + 3.0 <= t && t < i + 4.0 {
            let u = t - (i + 3.0);
            (-u.powi(3) + 3.0 * u.powi(2) - 3.0 * u + 1.0) / 6.0
        } else {
            0.0
        }
    })
}

fn get_curve<'a>(points: &'a Vec<[f64; 2]>) -> Box<dyn 'a + FnOnce(f64) -> [f64; 2]> {
    Box::new(move |t| {
        points
            .iter()
            .enumerate()
            .map(|(i, p)| [p[0] * get_basis(i)(t), p[1] * get_basis(i)(t)])
            .fold([0.0, 0.0], |n, i| [n[0] + i[0], n[1] + i[1]])
    })
}

const DISTANCE: f64 = 4.0;
const DELTA: f64 = 0.1;

#[derive(Debug)]
pub struct Curve<const M: usize, const N: usize> {
    points: Vec<[f64; 2]>,
    lines: Option<Vec<[f64; 2]>>,
}
impl<const M: usize, const N: usize> Curve<M, N> {
    pub fn new() -> Self {
        Curve {
            points: Vec::new(),
            lines: None,
        }
    }
    pub fn push(&mut self, point: [f64; 2]) {
        self.points.push(point);
    }
    pub fn is_empty(&self) -> bool {
        self.points.is_empty()
    }
    pub fn iter(&self) -> std::slice::Iter<[f64; 2]> {
        self.points.iter()
    }
    pub fn get_lines(&self) -> Option<&Vec<[f64; 2]>> {
        self.lines.as_ref()
    }
    pub fn catch(&self, goal: [f64; 2]) -> Option<&[f64; 2]> {
        for point in &self.points {
            if (goal[0] - point[0]).abs() <= DISTANCE
                && (goal[1] - point[1]).abs() <= DISTANCE
                && (goal[0] - point[0]).powi(2) + (goal[1] - point[1]).powi(2) <= DISTANCE.powi(2)
            {
                return Some(point);
            }
        }
        None
    }
    pub fn line(&mut self) {
        let mut ans = Vec::new();
        let mut t: f64 = 3.0;
        while t < self.points.len() as f64 {
            ans.push(get_curve(&self.points)(t));
            t += DELTA;
        }
        self.lines = Some(ans);
    }
}

impl<const M: usize, const N: usize> std::ops::Index<usize> for Curve<M, N> {
    type Output = [f64; 2];
    fn index(&self, index: usize) -> &Self::Output {
        &self.points[index]
    }
}

impl<const M: usize, const N: usize> IntoIterator for Curve<M, N> {
    type Item = [f64; 2];
    type IntoIter = std::vec::IntoIter<[f64; 2]>;
    fn into_iter(self) -> Self::IntoIter {
        self.points.into_iter()
    }
}
