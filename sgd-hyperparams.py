import numpy as np
from tqdm import tqdm
from sklearn.linear_model import SGDClassifier
try:
    from sklearn.utils._testing import ignore_warnings
except ImportError:
    from sklearn.utils.testing import ignore_warnings
from sklearn.exceptions import ConvergenceWarning
from sklearn.datasets import make_blobs
from sklearn.pipeline import make_pipeline
from sklearn.preprocessing import StandardScaler
from pmlb import classification_dataset_names, fetch_data
import concurrent.futures
import itertools

# These are the hyper-parameters to search (cartesian product)
SEEDS = range(220, 224)
ETA0S = [10.0, 100.0]   # [0.001 * 10**e for e in range(0, 6)]
MAXITER = [40000, 80000] # [20000, 40000, 80000]
TOL = [0.001]     # [0.001, 0.01, 0.1]

datasets = classification_dataset_names
results = {}

def grad(w, X, y, i):
  return y[i]*np.dot(w, X[i]) >= 1

class DatasetExitEarly(Exception):
    def __init__(self, dataset, classes, seed, eta0, maxiter, tol):
        self.dataset = dataset
        self.classes = classes
        self.seed = seed
        self.eta0 = eta0
        self.maxiter = maxiter
        self.tol = tol

@ignore_warnings(category=ConvergenceWarning)
def fit_and_check(data, c1, c2, seed, eta0, maxiter, tol):
    # Project two classes out of a dataset with potentially more
    X = np.array(data.loc[data['target'].isin([c1, c2]), data.columns != 'target']).astype(int)
    y = np.array(data.loc[data['target'].isin([c1, c2])]['target'].map(lambda x: 1 if x == c1 else -1)).astype(int)
    if(len(X) != len(y)):
        raise Exception("Error: wrong sizes of X = {} and y = {}".format(len(X), len(y)))
    clf = make_pipeline(StandardScaler(),
      SGDClassifier(loss="squared_hinge", alpha = 0.0, eta0 = eta0, learning_rate='adaptive', max_iter=maxiter, tol=tol, random_state=seed, fit_intercept=True))
    clf.fit(X, y)
    S = np.all([ grad(clf.steps[1][1].coef_.astype(int), X, y, i) for i in range(len(y)) ])
    return S

try:
    for dataset in tqdm(datasets):
        data = fetch_data(dataset)
        # if (data.shape[1] > 100 or data.shape[0] > 400):
        #    continue
        all_classes = set(data['target'])
        classes = [(c1, c2) for (c1, c2) in itertools.product(all_classes, all_classes) if c1 < c2]

        with concurrent.futures.ThreadPoolExecutor(max_workers=32) as executor:
            jobs = {
                executor.submit(fit_and_check, data, c1, c2, seed, eta0, maxiter, tol): ((c1, c2), seed, eta0, maxiter, tol) 
                  for ((c1, c2), seed, eta0, maxiter, tol) in itertools.product(classes, SEEDS, ETA0S, MAXITER, TOL)
            }
            try:
                for future in concurrent.futures.as_completed(jobs):
                    (classes, seed, eta0, maxiter, tol) = jobs[future]
                    if future.result():
                        raise DatasetExitEarly(dataset, classes, seed, eta0, maxiter, tol)

            except DatasetExitEarly as d:
                results[d.dataset] = (d.classes, d.seed, d.eta0, d.maxiter, d.tol)
                print(results)
                continue

except Exception as e:
    print(results)
    print("Exception (Hiding stacktrace): ", e)

print(results)

